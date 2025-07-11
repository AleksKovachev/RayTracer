#include "Camera.h" // Camera
#include "Colors.h" // Color, ColorMode, Colors::Black
#include "Lights.h" // Light, PointLight
#include "Mesh.h" // PreparedMesh
#include "Rays.h" // Ray, RayType
#include "Render.h"
#include "RenderSettings.h" // RenderMode, IntersectionData, Settings
#include "Scene.h" // Scene, Mesh
#include "Triangle.h" // Triangle
#include "utils.h" // writeColorToFile, areEqual, isGreaterEqualThan, isLessEqualThan
#include "Vectors.h" // FVector2, FVector3

#include <algorithm> // round, min, max
#include <filesystem> // create_directories
#include <fstream> // ofstream, std::ios::binary
#include <iostream> // cout, flush
#include <limits> // std::numeric_limits<float>::max
#include <numbers> // pi_v
#include <string> // string, to_string


Render::Render( const Scene& scene )
    : m_scene{ scene },
    m_overrideCamera{ nullptr },
    m_overrideSaveName{ nullptr },
    m_frames { 1 } {
}

Render::Render( const Scene& scene, Camera& overrideCamera )
    : m_scene{ scene },
    m_overrideCamera{ &overrideCamera },
    m_overrideSaveName{ nullptr },
    m_frames{ 1 } {
}

Render::Render( const Scene& scene, Camera& overrideCamera, const std::string& overrideSaveName )
    : m_scene{ scene },
    m_overrideCamera{ &overrideCamera },
    m_overrideSaveName{ &overrideSaveName },
    m_frames{ 1 } {
}

Render::~Render() {
    delete m_overrideCamera;
    delete m_overrideSaveName;
}

bool Render::IsInShadow(
    const FVector3& lightDir,
    const IntersectionData& data,
    const FVector3& surfaceNormal,
    const float distToLight
) {
    /* Define a small epsilon to avoid self - intersection artifacts. Often
     * 1e-3 - 1e-5 is used for ray origins. Smaller value if scene scale is
     * tiny. A good approach is to have an adaptive shadow bias, based on the
     * size of the Triangle hit or the size of the object. Another common
     * technique is to use dot product of the ray direction and the surface
     * Normal. If the light ray is nearly parallel - the bias shuold be higher */
    const float shadowBias{ data.scene.GetSettings().shadowBias }; // Higher values create artifacts

    /* Offset the hitPoint slightly along the normal to avoid self - intersection
     * Another common technique is to check rayPointDist > EPSILON */
    const FVector3 offsetHitPoint{ data.intersectionPt + (surfaceNormal * shadowBias) };

    for ( const PreparedMesh& mesh : data.meshes ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // If Ray is parallel - Ignore, it can't be hit.
            float rayProj = lightDir.Dot( triangle.GetNormal() );
            if ( areEqual( rayProj, 0.f ) )
                continue;

            // If rayProj > 0, ray is pointing towards triangle back face.
            // If rayProj < 0, ray is pointing towards triangle front face.

            float rayPlaneDist = (triangle.GetVert( 0 ).pos - offsetHitPoint)
                .Dot( triangle.GetNormal() );
            float rayPointDist = rayPlaneDist / rayProj; // Ray-to-Point scale factor

            /* Check if: Ray hits behind the hitPoint(negative rayPointDist)
             *           Ray hits at or very near the hitPoint (self-intersection)
             *           Ray hits beyond the light source */
            if ( isLessThan( rayPointDist, shadowBias ) )
                continue; // This intersection is not valid for shadow casting

            /* Geometry that's on the other side of the
             * light source shouldn't cast shadows here */
            if ( rayPointDist > distToLight )
                continue;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = offsetHitPoint + (lightDir * rayPointDist);

            if ( triangle.IsPointInside( intersectionPt ) )
                return true;
        }
    }
    return false;
}

Color Render::Shade(
    const IntersectionData& intersectionData,
    const Triangle& triangle,
    const FVector3* hitNormal
) {
    float R{};
    float G{};
    float B{};
    Color pixelColor{};
    Color renderColor{};

    const FVector3& surfaceNormal = (hitNormal == nullptr ? triangle.GetNormal() : *hitNormal);

    for ( const Light* light : intersectionData.scene.GetLights() ) {
        //! If Scene lights get more types, replace static_cast with dynamic to check light type.
        const PointLight* ptLight = static_cast<const PointLight*>(light);

        // Compute Light Direction
        FVector3 lightDir = ptLight->GetPosition() - intersectionData.intersectionPt;

        // Compute distance from intersection point to light source (sphere radius)
        float lightDirLen = lightDir.GetLength();

        // Normalize vector
        lightDir.NormalizeInPlace();

        // Avoid 0-division after falloff calculation
        if ( areEqual( lightDirLen, 0.f ) )
            continue;

        // Negative numbers are 0 in color. Positive numbers above 1 are clipped
        // Calculate the Cosine Law
        float cosLaw = std::max( 0.f, lightDir.Dot( surfaceNormal ) );
        if ( areEqual( cosLaw, 0.f ) )
            continue;

        // Compute sphere area
        float falloff = 4 * std::numbers::pi_v<float> *lightDirLen * lightDirLen;

        // If there's something on the way to the light - leave the color as is and continue
        if ( Render::IsInShadow( lightDir, intersectionData, surfaceNormal, lightDirLen ) )
            continue;

        // Cibsuder light Intensity and falloff in the value
        cosLaw *= ptLight->GetIntensity() / falloff;

        //! Clamp here to make 1.0 the max intensity!
        // cosLaw = std::min( 1.f, cosLaw );

        R += cosLaw;
    }

    if ( areEqual( R, 0.f ) )
        return Colors::Black;

    G = B = R;

    renderColor = intersectionData.scene.GetSettings().colorMode == ColorMode::RandomTriangleColor
        ? triangle.color : intersectionData.currMeshMat.albedo;

    // multiply by normalized albedo
    const int& maxComp = intersectionData.scene.GetSettings().maxColorComp;
    R *= static_cast<float>(renderColor.r) / maxComp;
    G *= static_cast<float>(renderColor.g) / maxComp;
    B *= static_cast<float>(renderColor.b) / maxComp;

    //! Clamp here to make everything > 1.0 clip back to 1.0!
    pixelColor.r = static_cast<int>(round( std::min( 1.f, R ) * maxComp ));
    pixelColor.g = static_cast<int>(round( std::min( 1.f, G ) * maxComp ));
    pixelColor.b = static_cast<int>(round( std::min( 1.f, B ) * maxComp ));

    return pixelColor;
}

FVector2 calculateBarycentricCoords( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector3 v0p = intersectionPt - triangle.GetVert( 0 ).pos;
    const FVector3 v0v2 = triangle.GetVert( 2 ).pos - triangle.GetVert( 0 ).pos;
    const FVector3 v0v1 = triangle.GetVert( 1 ).pos - triangle.GetVert( 0 ).pos;
    const float U = (v0p * v0v2).GetLength() / (triangle.GetArea() * 2);
    const float V = (v0v1 * v0p).GetLength() / (triangle.GetArea() * 2);
    return { U, V };
}

FVector3 calculateHitNormal( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector2 UV{ calculateBarycentricCoords( intersectionPt, triangle ) };
    return triangle.GetVert( 1 ).normal * UV.x
        + triangle.GetVert( 2 ).normal * UV.y
        + triangle.GetVert( 0 ).normal * (1 - UV.x - UV.y);
}

//IntersectionData traceRay( const Ray& ray, const float maxT = std::numeric_limits<float>::max() ) const;


Color Render::GetTriangleIntersection(
    Ray& ray,
    const Scene& scene,
    const unsigned reflectionDepth
) const {
    Color pixelColor{ scene.GetSettings().BGColor };
    float closestIntersectionP{ std::numeric_limits<float>::max() };

    for ( const auto& mesh : scene.GetPreparedMeshes() ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // rayProj = 0 -> Ray is parallel to surface - Ignore, it can't hit.
            // rayProj > 0 -> back-face
            // rayProj < 0 -> front-face
            float rayProj = ray.direction.Dot( triangle.GetNormal() );
            if ( isGreaterEqualThan( rayProj, 0.f ) ) // Ignore back-face
                continue;

            float rayPlaneDist = (triangle.GetVert( 0 ).pos - ray.origin)
                .Dot( triangle.GetNormal() );

            if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
                // Ray is not towards Triangle's plane
                continue;

            // Ray-to-Point scale factor for unit vector to reach the Point (t)
            float rayPointDist = rayPlaneDist / rayProj;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = ray.origin + (ray.direction * rayPointDist);

            // Ignore intersection if a closer one to the Camera has already been found
            if ( rayPointDist > closestIntersectionP || isLessEqualThan( rayPointDist, 0.f ) )
                continue;

            // If the Plane intersection point is not inside the triangle - don't render it
            if ( !triangle.IsPointInside( intersectionPt ) )
                continue;

            closestIntersectionP = rayPointDist;

            if ( scene.GetRenderMode() == RenderMode::ObjectColor ) {
                switch ( scene.GetSettings().colorMode ) {

                    case ColorMode::RandomTriangleColor: {
                        pixelColor = triangle.color;
                        break;
                    }
                    case (ColorMode::LoadedMaterial):
                    case (ColorMode::RandomMeshColor): {
                        pixelColor = mesh.m_material.albedo;
                        break;
                    }
                }
                continue;
            } else if ( scene.GetRenderMode() == RenderMode::Barycentric ) {
                const FVector2 UV = calculateBarycentricCoords( intersectionPt, triangle );
                pixelColor = { UV.x, UV.y, 0.f }; // Display Barycentric Coordinates
                continue;
            }

            // Shade Flat
            IntersectionData intersectionData(
                scene.GetPreparedMeshes(), mesh.m_material, scene, intersectionPt );

            switch ( mesh.m_material.type ) {
                case MaterialType::Diffuse: {
                    if ( mesh.m_material.smoothShading ) {
                        FVector3 hitNormal = calculateHitNormal( intersectionPt, triangle );
                        pixelColor = Shade( intersectionData, triangle, &hitNormal );
                    } else {
                        pixelColor = Shade( intersectionData, triangle, nullptr );
                    }
                    break;
                }
                case MaterialType::Reflective: {
                    // Negative depth means current ray doesn't support it.
                    if ( reflectionDepth != 0 ) {
                        FVector3 useNormal{ mesh.m_material.smoothShading ?
                            calculateHitNormal( intersectionPt, triangle ) :
                            useNormal = triangle.GetNormal()
                        };

                        // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
                        FVector3 reflectionDir = ray.direction - (
                            useNormal * ray.direction.Dot( useNormal ) * 2);

                        // If current ray has no support for depth - construct
                        // this one with maximum depth from the settings.
                        int reflectDepth = ray.pathDepth < 0
                            ? scene.GetReflectionDepth() : ray.pathDepth;

                        Ray reflectionRay{
                            intersectionPt,
                            reflectionDir,
                            reflectDepth,
                            RayType::Reflective
                        };

                        pixelColor = GetTriangleIntersection(
                            reflectionRay,
                            scene,
                            reflectionRay.pathDepth--
                        );
                        pixelColor *= mesh.m_material.albedo;
                    } else {
                        pixelColor = Colors::Black;
                    }
                    break;
                }
            } // MaterialType
        }
    }

    return pixelColor;
}

void Render::RenderImage() {
    const int& width{ m_scene.GetSettings().renderWidth };
    const int& height{ m_scene.GetSettings().renderHeight };
    const Camera& camera{
        m_overrideCamera == nullptr ? m_scene.GetCamera() : *m_overrideCamera };
    std::ofstream ppmFileStream = PrepareScene();

    for ( int y{}; y < height; ++y ) {
        for ( int x{}; x < width; ++x ) {
            Ray ray = camera.GenerateRay( x, y );
            Color pixelColor = GetTriangleIntersection( ray, m_scene, ray.pathDepth );
            writeColorToFile( ppmFileStream, pixelColor );
        }
        std::cout << "\rLine: " << y + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();
}

void Render::RenderCameraMoveAnimation(
    const FVector3& initialPos, const FVector3& moveWith ) {

    Camera camera{};
    m_overrideCamera = &camera;
    camera.Move( initialPos );
    std::string saveName;

    for ( int frame{}; frame < m_frames; ++frame ) {
        camera.Move( moveWith );
        saveName = "Move" + std::to_string( frame );
        RenderImage();
    }
}

void Render::RenderRotationAroundObject( const FVector3& initialPos, const FVector3& rot ) {
    Camera camera{};
    m_overrideCamera = &camera;
    std::string saveName;

    camera.Move( initialPos );
    float distToObject{ initialPos.z };

    for ( int frame{}; frame < m_frames; ++frame ) {
        camera.RotateAroundPoint(
            { 0.f, 0.f, distToObject }, { frame * rot.x, frame * rot.y, frame * rot.z } );
        saveName = "Orbit" + std::to_string( frame + 1 );
        RenderImage();
    }
}

std::ofstream Render::PrepareScene() {
    const Settings& settings{ m_scene.GetSettings() };

    const std::string& saveDir{ settings.saveDir };
    const std::string& saveName{
        (m_overrideSaveName == nullptr ? settings.saveName : *m_overrideSaveName) };

    std::filesystem::create_directories( saveDir );
    std::ofstream ppmFileStream( saveDir + "/" + saveName + ".ppm", std::ios::binary );
    ppmFileStream << "P6\n";
    ppmFileStream << settings.renderWidth << " " << settings.renderHeight << "\n";
    ppmFileStream << settings.maxColorComp << "\n";

    return ppmFileStream;
}