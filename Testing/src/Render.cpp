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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // stbi_load

#include <algorithm> // round, min, max, swap
#include <cassert> // assert
#include <cmath> // sqrtf
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

        // Skip shadowing meshes with refractive materials.
        if ( mesh.m_material.type == MaterialType::Refractive )
            continue;

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
            return data.material->texture.albedo;
        }
    }

    // Should never get here.
    return Colors::Black;
}

FVector2 Render::CalcBaryCoords( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector3 v0p = intersectionPt - triangle.GetVert( 0u ).pos;
    const FVector3 v0v2 = triangle.GetVert( 2u ).pos - triangle.GetVert( 0u ).pos;
    const FVector3 v0v1 = triangle.GetVert( 1u ).pos - triangle.GetVert( 0u ).pos;
    const float U = (v0p * v0v2).GetLength() / (triangle.GetArea() * 2);
    const float V = (v0v1 * v0p).GetLength() / (triangle.GetArea() * 2);
    return { U, V };
}

Color Render::ShadeBary( const IntersectionData& data ) const {
    const FVector2 UV = CalcBaryCoords( data.hitPoint, data.triangle );
    return { UV.x, UV.y, 0.f }; // Display Barycentric Coordinates.
}


FVector3 Render::CalcHitNormal( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector2 UV{ CalcBaryCoords( intersectionPt, triangle ) };
    return (triangle.GetVert( 1u ).normal * UV.x
        + triangle.GetVert( 2u ).normal * UV.y
        + triangle.GetVert( 0u ).normal * (1 - UV.x - UV.y)).Normalize();
}

Color Render::GetEdgesColor(const IntersectionData& data) {
    const FVector2 UV = CalcBaryCoords( data.hitPoint, data.triangle );
    const float edgeWdith = data.material->texture.scalar;
    if ( UV.x < edgeWdith || UV.y < edgeWdith || 1 - UV.x - UV.y < edgeWdith ) {
        return data.material->texture.colorA;
    } else {
        return data.material->texture.colorB;
    }
}

Color Render::GetCheckerColor( const IntersectionData& data ) {
    const FVector2 bary = CalcBaryCoords( data.hitPoint, data.triangle );

    FVector3 UVW{ data.triangle.GetVert( 1 ).UVCoords * bary.x
        + data.triangle.GetVert( 2 ).UVCoords * bary.y
        + data.triangle.GetVert( 0 ).UVCoords * (1.f - bary.x - bary.y) };

    const float squareSize = data.material->texture.scalar;
    int checkU = static_cast<int>(std::floor( UVW.x / squareSize ));
    int checkV = static_cast<int>(std::floor( UVW.y / squareSize ));

    if ( (checkU + checkV) % 2 == 0 ) {
        return data.material->texture.colorA;
    } else {
        return data.material->texture.colorB;
    }
}

Color Render::ShadeDiffuse( const IntersectionData& data ) const {
    FVector3 hitNormal{};
    Color pixelColor{};
    Color renderColor{};

    if ( data.material->smoothShading )
        hitNormal = CalcHitNormal( data.hitPoint, data.triangle );

    const FVector3& surfaceNormal = (data.material->smoothShading ? hitNormal : data.faceNormal );

    switch ( data.material->texType ) {
        case TextureType::RedGreenEdgesP: {
            renderColor = GetEdgesColor( data );
            break;
        }
        case TextureType::BlackWhiteCheckerP: {
            renderColor = GetCheckerColor( data );
            break;
        }
        case TextureType::Bitmap: {
            int width, height, channels;
            unsigned char* image = stbi_load(
                data.material->texture.filePath.c_str(), &width, &height, &channels, 0 );
            break;
        }
        case TextureType::ColorTexture: {
            renderColor = m_scene.GetSettings().colorMode == ColorMode::RandomTriangleColor
                ? data.triangle.color : data.material->texture.albedo;
            break;
        }
        default: {
            assert( false );
        }
    }

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
        Ray shadowRay{ offsetHitPoint, lightDir, -1, RayType::Shadow, false };

        if ( Render::IsInShadow( shadowRay, lightDirLen ) )
            // If there's something on the way to the light - leave the color as is.
            continue;

        // Cibsuder light Intensity and falloff in the value.
        cosLaw *= ptLight->GetIntensity() / falloff;

        //! Clamp here to make 1.0 the max intensity!
        // cosLaw = std::min( 1.f, cosLaw );

        pixelColor.r += cosLaw;
    }

    if ( areEqual( pixelColor.r, 0.f ) )
        return Colors::Black;

    pixelColor.g = pixelColor.b = pixelColor.r;

    // Multiply by albedo or procedural texture.
    pixelColor.r *= renderColor.r;
    pixelColor.g *= renderColor.g;
    pixelColor.b *= renderColor.b;

    return pixelColor;
}

Color Render::ShadeReflective( const Ray& ray, const IntersectionData& data ) const {
    FVector3 useNormal{ data.material->smoothShading ?
        CalcHitNormal( data.hitPoint, data.triangle ) : data.faceNormal
    };

    // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
    FVector3 reflectionDir = ray.direction - (
        useNormal * ray.direction.Dot( useNormal ) * 2);

    // If current ray has no support for depth - construct a fresh one.
    int reflectDepth = ray.pathDepth < 0 ? 0 : ray.pathDepth;

    Ray reflectionRay{
        data.hitPoint,
        reflectionDir.Normalize(),
        reflectDepth + 1,
        RayType::Reflective,
        false
    };

    IntersectionData intersectData = TraceRay( reflectionRay );
    Color pixelColor = Shade( reflectionRay, intersectData );
    pixelColor *= data.material->texture.albedo;

    return pixelColor;
}

float Render::CalcFresnelSchlickApprox( const float ior1, const float ior2, const float dotIN ) {
    float ratioIORs{ ior1 / ior2 };
    float cosIN = -dotIN; // cos(A)
    float R0 = std::pow( (ior1 - ior2) / (ior1 + ior2), 2.0f );
    // Use the cosine of the angle *inside* the denser material for the Fresnel term.
    // If ray was entering (original dotIN <= 0), use cosIN.
    // If ray was exiting (original dotIN > 0), use cosRnN.
    float cosRnN = std::sqrtf( 1.f - ratioIORs * ratioIORs * (1.f - (cosIN * cosIN)) );
    float cosTermForFresnel = (dotIN > 0) ? cosRnN : cosIN;
    return R0 + (1.0f - R0) * std::pow( (1.0f - cosTermForFresnel), 5.0f );
}

Color Render::ShadeRefractive( const Ray& ray, const IntersectionData& data ) const {
    /* Namings used:
    * dotIN: dot product of incident ray with surface normal.
    * cosIN: cosine of the incident ray with the surface normal.
    * sinRnN: sine between the refraction ray and the negative surface normal.
    */

    FVector3 useNormal{ data.material->smoothShading ?
        CalcHitNormal( data.hitPoint, data.triangle ) : data.faceNormal
    };

    float ior1{ 1.f }; // air, hard-coded for now
    float ior2{ data.material->ior };
    float dotIN = ray.direction.Dot( useNormal );

    // Check if the incident ray enters or leaves the object.
    if ( dotIN > 0 ) {
        // Leaves - swap IOR values and use inverse normal.
        useNormal = -useNormal;
        dotIN = -dotIN;
        std::swap( ior1, ior2 );
    }

    // If current ray has no support for depth - construct a fresh one.
    int pathDepth = ray.pathDepth < 0 ? 0 : ray.pathDepth;

    /* Compute the cosine between the incident ray and the normal vector.
    * cos(I, N) = -dot(I, N) = dot(I, -N). */
    float cosIN = -dotIN; //!? cos(A)

    float eta{ ior1 / ior2 };
    float sin2ThetaT = eta * eta * (1.f - (cosIN * cosIN));

    Color refractionColor{};
    // Default to 100% reflection for Total Internal Reflection (TIR)
    float fresnel = 1.f;

    // Check if TIR occurs and don't calculate refraction if it does.
    //if ( std::sqrtf( 1.f - cosIN * cosIN ) <= ior2 / ior1 ) {
    if ( sin2ThetaT < 1.f ) {
        /* Using Snell's Law, find sin(R, -N), R - refraction ray.
         * sin(R, -N) = (sqrt(1 - cos^2(I, N)) * ior1) / ior2 */
        float sinRnN{ (sqrtf( 1.f - (cosIN * cosIN) ) * ior1) / ior2 }; //!? sin(B)
        float cosRnN{ std::sqrtf( 1.0f - (sinRnN * sinRnN) ) }; //!? cos(B)

        // Compute the Refraction Ray direction.
        FVector3 A = -useNormal * cosRnN;
        FVector3 C = (ray.direction + (useNormal * cosIN)).Normalize();
        FVector3 B = C * sinRnN;

        // Generate Refraction Ray.
        Ray refractionRay{
            data.hitPoint + (-useNormal * m_scene.GetSettings().refractBias),
            (A + B).Normalize(),
            //((ray.direction * eta) + (useNormal * (eta * cosIN - cosRnN))).Normalize(),
            pathDepth + 1,
            RayType::Refractive,
            false
        };

        // Trace Refraction Ray.
        IntersectionData refractData{ TraceRay( refractionRay ) };
        refractionColor = Shade( refractionRay, refractData );

        //fresnel = CalcFresnelSchlickApprox( ior1, ior2, dotIN );
        fresnel = 0.5f * std::pow( (1.0f + dotIN), 5.f );
    }

    // Generate Reflection Ray.
    Ray reflectionRay{
        data.hitPoint + (useNormal * m_scene.GetSettings().refractBias),
        ray.direction - (useNormal * ray.direction.Dot( useNormal ) * 2.f),
        pathDepth + 1,
        RayType::Reflective,
        false
    };
    reflectionRay.direction.NormalizeInPlace();

    // Generate Reflection Ray.
    IntersectionData reflectData{ TraceRay( reflectionRay ) };
    Color reflectionColor = Shade( reflectionRay, reflectData );

    return reflectionColor * fresnel + refractionColor * (1.f - fresnel);
}

Color Render::Shade( const Ray& ray, const IntersectionData& data ) const {
    Color pixelColor{ m_scene.GetSettings().BGColor };

    if ( data.material == nullptr ) {
        // The camera ray didn't hit any objects - just the background.
        return pixelColor;
    }
    else if ( ray.pathDepth >= m_scene.GetSettings().pathDepth ) {
        return pixelColor; // or  Colors::Black
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
        //pixelColor = ShadeDiffuse( data );
        pixelColor = ShadeReflective( ray, data );
    }
    else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Refractive ) {
        //pixelColor = ShadeDiffuse( data );
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
    float bias{ 0.f }; //! Default value currently only used for reflection!

    if ( ray.type == RayType::Shadow )
        bias = m_scene.GetSettings().shadowBias;
    else if (ray.type == RayType::Refractive )
        bias = m_scene.GetSettings().refractBias;

    for ( const PreparedMesh& mesh : m_scene.GetPreparedMeshes() ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // rayProj = 0 -> Ray is parallel to surface - Ignore, it can't hit.
            // rayProj > 0 -> back-face
            // rayProj < 0 -> front-face
            float rayProj = ray.direction.Dot( triangle.GetNormal() );

            if ( ray.ignoreBackface ) {
                if ( isGreaterEqualThan( rayProj, 0.f ) )
                    continue;
            }
            else {
                if ( areEqual( rayProj, 0.f ) )
                    continue;
            }

            float rayPlaneDist = (triangle.GetVert( 0u ).pos - ray.origin)
                .Dot( triangle.GetNormal() );

            // Ray-to-Point scale factor for unit vector to reach the Point (t).
            float rayPointDist = rayPlaneDist / rayProj;

            if ( ray.ignoreBackface ) {
                if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
                    // Ray is not towards Triangle's plane.
                    continue;
            }
            else {
                /* Check if: Ray hits behind the hitPoint(negative rayPointDist)
                 *           Ray hits at or very near the hitPoint (self-intersection)
                 *           Ray hits beyond the light source */
                if ( isLessThan( rayPointDist, bias ) )
                    continue; // This intersection is not valid for shadow casting.
            }

            /* With shadow rays, maxT is the distance to the light source.
             * Geometry on the other side of the light shouldn't cast shadows here.
             * Currently maxT is only used for light distance. In the future it
             * will be used for other optimizations as well. */
            if ( rayPointDist > maxT || isLessEqualThan( rayPointDist, 0.f ) )
                continue;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = ray.origin + (ray.direction * rayPointDist);

            // Ignore intersection if a closer one to the Camera has already been found.
            if ( rayPointDist > closestIntersectionP )
                continue;

            // If the Plane intersection point is not inside the triangle - don't render it.
            if ( !triangle.IsPointInside( intersectionPt ) )
                continue;

            if ( ray.type == RayType::Shadow )
                intersectData; //TODO: Not finished! Make it work in place of IsInShadow()

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
            Ray ray = camera.GenerateRay( x, y, m_scene.GetSettings() );
            IntersectionData intersectData = TraceRay( ray );
            Color pixelColor = Shade( ray, intersectData );
            writeColorToFile( ppmFileStream, pixelColor, m_scene.GetSettings().maxColorComp );
        }
        std::cout << "\rLine: " << y + 1 << " / " << height << "  |  "
            << (static_cast<float>(y + 1) / height) * 100 << "%          " << std::flush;
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