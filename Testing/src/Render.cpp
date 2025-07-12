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
#include <cassert> // assert
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

bool Render::IsInShadow( const Ray& ray, const float distToLight ) const {
    for ( const PreparedMesh& mesh : m_scene.GetPreparedMeshes() ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // If Ray is parallel - Ignore, it can't be hit.
            float rayProj = ray.direction.Dot( triangle.GetNormal() );
            if ( areEqual( rayProj, 0.f ) )
                continue;

            // If rayProj > 0, ray is pointing towards triangle back face.
            // If rayProj < 0, ray is pointing towards triangle front face.

            float rayPlaneDist = (triangle.GetVert( 0u ).pos - ray.origin)
                .Dot( triangle.GetNormal() );
            float rayPointDist = rayPlaneDist / rayProj; // Ray-to-Point scale factor

            /* Check if: Ray hits behind the hitPoint(negative rayPointDist)
             *           Ray hits at or very near the hitPoint (self-intersection)
             *           Ray hits beyond the light source */
            if ( isLessThan( rayPointDist, m_scene.GetSettings().shadowBias ) )
                continue; // This intersection is not valid for shadow casting.

            // Geometry on the other side of the light shouldn't cast shadows here.
            if ( rayPointDist > distToLight )
                continue;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = ray.origin + (ray.direction * rayPointDist);

            if ( triangle.IsPointInside( intersectionPt ) )
                return true;
        }
    }
    return false;
}

Color Render::ShadeConstant( const IntersectionData& data ) const {
    switch ( m_scene.GetSettings().colorMode ) {

        case ColorMode::RandomTriangleColor: {
            return data.triangle.color;
        }
        case (ColorMode::LoadedMaterial):
        case (ColorMode::RandomMeshColor): {
            return data.material->albedo;
        }
    }

    // Should never get here.
    return Colors::Black;
}

FVector2 calculateBarycentricCoords( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector3 v0p = intersectionPt - triangle.GetVert( 0u ).pos;
    const FVector3 v0v2 = triangle.GetVert( 2u ).pos - triangle.GetVert( 0u ).pos;
    const FVector3 v0v1 = triangle.GetVert( 1u ).pos - triangle.GetVert( 0u ).pos;
    const float U = (v0p * v0v2).GetLength() / (triangle.GetArea() * 2);
    const float V = (v0v1 * v0p).GetLength() / (triangle.GetArea() * 2);
    return { U, V };
}

Color Render::ShadeBary( const IntersectionData& data ) const {
    const FVector2 UV = calculateBarycentricCoords( data.hitPoint, data.triangle );
    return { UV.x, UV.y, 0.f }; // Display Barycentric Coordinates.
}


FVector3 calcHitNormal( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector2 UV{ calculateBarycentricCoords( intersectionPt, triangle ) };
    return triangle.GetVert( 1u ).normal * UV.x
        + triangle.GetVert( 2u ).normal * UV.y
        + triangle.GetVert( 0u ).normal * (1 - UV.x - UV.y);
}

Color Render::ShadeDiffuse( const IntersectionData& data ) const {
    FVector3 hitNormal{};
    float R{};
    float G{};
    float B{};
    Color pixelColor{};
    Color renderColor{};

    if ( data.material->smoothShading )
        hitNormal = calcHitNormal( data.hitPoint, data.triangle );

    const FVector3& surfaceNormal = (data.material->smoothShading ? hitNormal : data.faceNormal );

    for ( const Light* light : m_scene.GetLights() ) {
        //! If Scene lights get more types, replace static_cast with dynamic to check light type.
        const PointLight* ptLight = static_cast<const PointLight*>(light);

        // Compute Light Direction.
        FVector3 lightDir = ptLight->GetPosition() - data.hitPoint;

        // Compute distance from intersection point to light source (sphere radius).
        float lightDirLen = lightDir.GetLength();

        // Normalize vector.
        lightDir.NormalizeInPlace();

        // Avoid 0-division after falloff calculation.
        if ( areEqual( lightDirLen, 0.f ) )
            continue;

        // Negative numbers are 0 in color. Positive numbers above 1 are clipped.
        // Calculate the Cosine Law.
        float cosLaw = std::max( 0.f, lightDir.Dot( surfaceNormal ) );
        if ( areEqual( cosLaw, 0.f ) )
            continue;

        // Compute sphere area.
        float falloff = 4 * std::numbers::pi_v<float> *lightDirLen * lightDirLen;

        /* Offset the hitPoint slightly along the normal to avoid self-intersection
         * Another common technique is to check rayPointDist > EPSILON */
        const FVector3 offsetHitPoint{
            data.hitPoint + (surfaceNormal * m_scene.GetSettings().shadowBias ) };
        Ray shadowRay{ offsetHitPoint, lightDir, -1, RayType::Shadow };

        if ( Render::IsInShadow( shadowRay, lightDirLen ) )
            // If there's something on the way to the light - leave the color as is.
            continue;

        // Cibsuder light Intensity and falloff in the value.
        cosLaw *= ptLight->GetIntensity() / falloff;

        //! Clamp here to make 1.0 the max intensity!
        // cosLaw = std::min( 1.f, cosLaw );

        R += cosLaw;
    }

    if ( areEqual( R, 0.f ) )
        return Colors::Black;

    G = B = R;

    renderColor = m_scene.GetSettings().colorMode == ColorMode::RandomTriangleColor
        ? data.triangle.color : data.material->albedo;

    // multiply by normalized albedo
    const int& maxComp = m_scene.GetSettings().maxColorComp;
    R *= static_cast<float>(renderColor.r) / maxComp;
    G *= static_cast<float>(renderColor.g) / maxComp;
    B *= static_cast<float>(renderColor.b) / maxComp;

    // Clamp here to make everything > 1.0 clip back to 1.0!
    pixelColor.r = static_cast<int>(round( std::min( 1.f, R ) * maxComp ));
    pixelColor.g = static_cast<int>(round( std::min( 1.f, G ) * maxComp ));
    pixelColor.b = static_cast<int>(round( std::min( 1.f, B ) * maxComp ));

    return pixelColor;
}

Color Render::ShadeReflective( const Ray& ray, const IntersectionData& data ) const {
    Color pixelColor{ m_scene.GetSettings().BGColor };

    if ( ray.pathDepth >= m_scene.GetReflectionDepth() )
        return Colors::Black;

    FVector3 useNormal{ data.material->smoothShading ?
        calcHitNormal( data.hitPoint, data.triangle ) :
        useNormal = data.faceNormal
    };

    // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
    FVector3 reflectionDir = ray.direction - (
        useNormal * ray.direction.Dot( useNormal ) * 2);

    // If current ray has no support for depth - construct a fresh one.
    int reflectDepth = ray.pathDepth < 0 ? 0 : ray.pathDepth;

    Ray reflectionRay{
        data.hitPoint,
        reflectionDir,
        reflectDepth++,
        RayType::Reflective
    };

    IntersectionData intersectData = TraceRay( reflectionRay );
    pixelColor = Shade( reflectionRay, intersectData );
    pixelColor *= data.material->albedo;

    return pixelColor;
}

Color Render::ShadeRefractive( const Ray& ray, const IntersectionData& data ) const {
    return {};
}

Color Render::Shade( const Ray& ray, const IntersectionData& data ) const {
    Color pixelColor{ m_scene.GetSettings().BGColor };

    if ( data.material == nullptr ) {
        // Teh camera ray didn't hit any objects - just the background.
        return pixelColor;
    }
    else if ( m_scene.GetRenderMode() == RenderMode::ObjectColor ) {
        pixelColor = ShadeConstant( data );
    }
    else if ( m_scene.GetRenderMode() == RenderMode::Barycentric ) {
        pixelColor = ShadeBary( data );
    }
    else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Diffuse ) {
        pixelColor = ShadeDiffuse( data );
    }
    else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Reflective ) {
        pixelColor = ShadeDiffuse( data );
        //pixelColor = ShadeReflective( ray, data );
    }
    else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Refractive ) {
        pixelColor = ShadeRefractive( ray, data );
    }
    else {
        assert( false );
    }

    return pixelColor;
}

IntersectionData Render::TraceRay( const Ray& ray, const float maxT ) const {
    IntersectionData intersectData{};
    float closestIntersectionP{ std::numeric_limits<float>::max() };

    for ( const PreparedMesh& mesh : m_scene.GetPreparedMeshes() ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // rayProj = 0 -> Ray is parallel to surface - Ignore, it can't hit.
            // rayProj > 0 -> back-face
            // rayProj < 0 -> front-face
            float rayProj = ray.direction.Dot( triangle.GetNormal() );
            if ( isGreaterEqualThan( rayProj, 0.f ) ) // Ignore back-face
                continue;

            float rayPlaneDist = (triangle.GetVert( 0u ).pos - ray.origin)
                .Dot( triangle.GetNormal() );

            if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
                // Ray is not towards Triangle's plane.
                continue;

            // Ray-to-Point scale factor for unit vector to reach the Point (t).
            float rayPointDist = rayPlaneDist / rayProj;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = ray.origin + (ray.direction * rayPointDist);

            // Ignore intersection if a closer one to the Camera has already been found.
            if ( rayPointDist > closestIntersectionP || isLessEqualThan( rayPointDist, 0.f ) )
                continue;

            // If the Plane intersection point is not inside the triangle - don't render it.
            if ( !triangle.IsPointInside( intersectionPt ) )
                continue;

            closestIntersectionP = rayPointDist;
            intersectData.faceNormal = triangle.GetNormal();
            intersectData.hitPoint = intersectionPt;
            intersectData.material = &mesh.m_material;
            intersectData.triangle = triangle;
        }
    }
    return intersectData;
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
            IntersectionData intersectData = TraceRay( ray );
            Color pixelColor = Shade( ray, intersectData );
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