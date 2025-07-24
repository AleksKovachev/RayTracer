#include "AccelerationStructures.h" // AABBox, AccTreeNode
#include "Camera.h" // Camera
#include "Colors.h" // Color, Colors::Black, Colors::Red
#include "ImageBuffer.h" // ImageBuffer
#include "Lights.h" // Light, PointLight
#include "Materials.h" // Bitmap, MaterialType, TextureType
#include "Rays.h" // Ray, RayType
#include "Render.h"
#include "Scene.h" // Scene
#include "Triangle.h" // Triangle
#include "utils.h" // writeColorToFile, areEqual, isGT

#include <algorithm> // max, min, round, swap
#include <cmath> // abs, ceil, pow, sqrtf
#include <filesystem> // create_directories
#include <functional> // ref
#include <iostream> // cerr, cout, flush, endl
#include <numbers> // pi_v
#include <stack> // stack
#include <stdexcept> // invalid_argument
#include <thread> // hardware_concurrency
#include <vector> // vector


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

void Render::RenderImage() {
    const unsigned& width{ m_scene.GetSettings().renderWidth };
    const unsigned& height{ m_scene.GetSettings().renderHeight };
    const Camera& camera{
        m_overrideCamera == nullptr ? m_scene.GetCamera() : *m_overrideCamera };
    std::ofstream ppmFileStream = PrepareScene();
    const AABBox& aabb{ m_scene.GetAABB() };
    Color pixelColor{};

    for ( unsigned y{}; y < height; ++y ) {
        for ( unsigned x{}; x < width; ++x ) {
            Ray ray = camera.GenerateRay( x, y, m_scene.GetSettings() );
            if ( !HasAABBCollision( ray, aabb ) ) {
                pixelColor = m_scene.GetSettings().BGColor;
            }
            else {
                IntersectionData intersectData = IntersectRay( ray );
                pixelColor = Shade( ray, intersectData );
            }
            writeColorToFile( ppmFileStream, pixelColor, m_scene.GetSettings().maxColorComp );
        }
        std::cout << "\rLine: " << y + 1 << " / " << height << "  |  "
            << (static_cast<float>(y + 1) / height) * 100 << "%          " << std::flush;
    }

    ppmFileStream.close();
}

void Render::RenderBuckets() {
    unsigned threadCount{ std::thread::hardware_concurrency() };

    if ( threadCount == 0 ) {
        std::cerr << "Warning: Could not detect number of hardware threads."
            "Falling back to 1.\n";
        threadCount = 1;
        RenderImage();
        return;
    }

    const unsigned& width = m_scene.GetSettings().renderWidth;
    const unsigned& height = m_scene.GetSettings().renderHeight;
    const Camera& camera{
        m_overrideCamera == nullptr ? m_scene.GetCamera() : *m_overrideCamera };
    std::ofstream ppmFileStream = PrepareScene();

    ImageBuffer buff{ width, height };

    std::queue<Bucket> buckets;
    std::mutex bucketMutex;


    unsigned bucketSize{ m_scene.GetSettings().bucketSize };
    if ( bucketSize == 0u ) {
        std::cerr << "Error: Buckets must have size. Defaulting to 24." << std::endl;
        bucketSize = 24u;
    }

    // Calculate number of rows and columns of buckets based on their size.
    unsigned cols = static_cast<unsigned>(std::ceil( static_cast<double>(width) / bucketSize ));
    unsigned rows = static_cast<unsigned>(std::ceil( static_cast<double>(height) / bucketSize ));

    for ( unsigned row = 0; row < rows; ++row ) {
        for ( unsigned col = 0; col < cols; ++col ) {
            unsigned startX = col * bucketSize;
            unsigned startY = row * bucketSize;
            unsigned endX = startX + bucketSize;
            unsigned endY = startY + bucketSize;

            // Clamp end coordinates to image boundaries.
            if ( endX > width )
                endX = width;
            if ( endY > height )
                endY = height;

            // Only add valid buckets.
            if ( startX < endX && startY < endY ) {
                buckets.push( Bucket( startX, startY, endX, endY ) );
            }
        }
    }

    std::vector<std::thread> threads{};
    threads.reserve( threadCount );

    for ( unsigned int i{}; i < threadCount; ++i ) {
        threads.emplace_back(
            &Render::RenderBucketWorker,
            this,
            std::ref( bucketMutex ),
            std::ref( buckets ),
            std::ref( buff )
        );
    }

    for ( std::thread& t : threads ) {
        if ( t.joinable() ) {
            t.join();
        }
    }

    for ( unsigned row{}; row < height; ++row ) {
        for ( unsigned col{}; col < width; ++col ) {
            writeColorToFile( ppmFileStream, buff[row][col], m_scene.GetSettings().maxColorComp );
        }
    }
    ppmFileStream.close();
}

void Render::RenderTree( const Bucket& region, ImageBuffer& buff ) {
    const Camera& camera{
        m_overrideCamera == nullptr ? m_scene.GetCamera() : *m_overrideCamera };
    Color pixelColor{ m_scene.GetSettings().BGColor };

    std::stack<int> nodeIndicesToCheck{};
    nodeIndicesToCheck.push( 0 ); // 0 is always the root index

    for ( unsigned row{ region.startY }; row < region.endY; ++row ) {
        for ( unsigned col{ region.startX }; col < region.endX; ++col ) {
            Ray ray = camera.GenerateRay( col, row, m_scene.GetSettings() );
            IntersectionData intersectData = IntersectRay( ray );
            if ( !intersectData.filled)
                pixelColor = m_scene.GetSettings().BGColor;
            else
                pixelColor = Shade( ray, intersectData );
            buff[row][col] = pixelColor;
        }
    }
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

IntersectionData Render::IntersectRay( const Ray& ray, const float maxT ) const {
    IntersectionData intersectData{};
    std::stack<int> nodeIndicesToCheck{};
    nodeIndicesToCheck.push( 0 ); // 0 is always the root index
    float closestIntersection{ std::numeric_limits<float>::max() };

    while ( nodeIndicesToCheck.size() > 0 ) {
        int nodeIdxToCHeck = nodeIndicesToCheck.top();
        nodeIndicesToCheck.pop();
        const AccTreeNode& currNode = m_scene.GetAccTree().GetNodes()[nodeIdxToCHeck];

        if ( !HasAABBCollision( ray, currNode.aabb ) )
            continue;

        if ( currNode.triIndices.size() > 0 ) { // leaf node
            currNode.Intersect(
                ray,
                maxT,
                closestIntersection,
                intersectData
            );
            // For shadow rays, one intersection is enough to cast a shadow.
            if ( ray.type == RayType::Shadow && intersectData.filled )
                break;
        } else {
            if ( currNode.children[0] != -1 )
                nodeIndicesToCheck.push( currNode.children[0] );
            if ( currNode.children[1] != -1 )
                nodeIndicesToCheck.push( currNode.children[1] );
        }
    }

    return intersectData;
}

void Render::RenderBucketWorker( std::mutex& bucketMutex, std::queue<Bucket>& buckets, ImageBuffer& buff ) {
    while ( true ) {
        Bucket bck;
        bool gotBucket = false;

        // Acquire lock to safely access the shared queue
        {
            std::lock_guard<std::mutex> lock( bucketMutex );
            if ( !buckets.empty() ) {
                bck = buckets.front();
                buckets.pop();
                gotBucket = true;
            }
        } // Lock released when going out of scope.

        if ( gotBucket )
            RenderTree( bck, buff );
        else
            break; // No more buckets. This thread can exit.

    }
}

bool Render::HasAABBCollision( const Ray& ray, const AABBox& aabb ) const {
    float tMin{ 0.f }; // entry, the closes intersection point along the ray must be positive.
    float tMax{ std::numeric_limits<float>::max() }; // exit

    for ( int i{}; i < 3; ++i ) {

        if ( areEqual( std::abs( ray.direction[i] ), 0.f ) ) { // parallel
            if ( ray.origin[i] < aabb.min[i] || ray.origin[i] > aabb.max[i] ) {
                return false; // Ray is outside the axis slab.
            }
            // Parallel and inside. tMin/tMax = -/+inf.
        }

        float t0 = (aabb.min[i] - ray.origin[i]) / ray.direction[i];
        float t1 = (aabb.max[i] - ray.origin[i]) / ray.direction[i];

        // Ensure t0 always holds the smaller of the 2 intersections.
        if ( isGT( t0, t1 ) )
            std::swap( t0, t1 );
        tMin = std::max( tMin, t0 );
        tMax = std::min( tMax, t1 );

        // If at any point tMin is greater than tMax, there's no intersection.
        if ( isGT( tMin, tMax ) )
            return false;
    }
    // t = tMin
    return true;
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

Color Render::ShadeNormals( const IntersectionData& data ) const {
    FVector3 hitNormal{};
    if ( data.material->smoothShading )
        hitNormal = CalcHitNormal( data.hitPoint, data.triangle );

    const FVector3& surfaceNormal = data.material->smoothShading ? hitNormal : data.faceNormal;

    return Color( surfaceNormal.x, surfaceNormal.y, surfaceNormal.z );
}

FVector3 Render::CalcHitNormal( const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector2 UV{ CalcBaryCoords( intersectionPt, triangle ) };
    return (triangle.GetVert( 1u ).normal * UV.x
        + triangle.GetVert( 2u ).normal * UV.y
        + triangle.GetVert( 0u ).normal * (1 - UV.x - UV.y)).Normalize();
}

Color Render::GetEdgesColor( const IntersectionData& data ) {
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

    if ( (checkU + checkV) % 2 == 0 )
        return data.material->texture.colorA;
    else
        return data.material->texture.colorB;
}

Color Render::GetBitmapColor( const IntersectionData& data ) {
    const FVector2 bary = CalcBaryCoords( data.hitPoint, data.triangle );

    FVector3 UVW{ data.triangle.GetVert( 1 ).UVCoords * bary.x
        + data.triangle.GetVert( 2 ).UVCoords * bary.y
        + data.triangle.GetVert( 0 ).UVCoords * (1.f - bary.x - bary.y) };

    const Bitmap& bitmap{ data.material->texture.bitmap };
    int rowIdx{ static_cast<int>((1 - UVW.y) * (bitmap.height - 1)) };
    int colIdx{ static_cast<int>(UVW.x * (bitmap.width - 1)) };
    int pixelIdx{ (rowIdx * bitmap.width + colIdx) * bitmap.channels };

    return { bitmap.buffer[pixelIdx], bitmap.buffer[pixelIdx + 1], bitmap.buffer[pixelIdx + 2] };
}

Color Render::GetRenderColor( const IntersectionData& data ) const {
    // Override if rendering geometry normals.
    if ( m_scene.GetRenderMode() == RenderMode::ShadedNormals ) {
        return ShadeNormals( data );
    }

    switch ( data.material->texture.type ) {
        case TextureType::EdgesP: {
            return GetEdgesColor( data );
        }
        case TextureType::CheckerP: {
            return GetCheckerColor( data );
        }
        case TextureType::Bitmap: {
            return GetBitmapColor( data );
        }
        case TextureType::SolidColor: {
            return m_scene.GetSettings().renderMode == RenderMode::RandomTriangleColor
                ? m_scene.GetTriangleColors()[data.triangle.colorIdx]
                : data.material->texture.albedo;
        }
        case TextureType::Invalid:
        default: {
            throw std::invalid_argument( "Unsupported texture type." );
        }
    }
    return Colors::Red; // Should never get here. Placed to silence compiler.
}

Color Render::ShadeDiffuse( const IntersectionData& data ) const {
    FVector3 hitNormal{};
    Color pixelColor{};
    Color renderColor{ GetRenderColor( data ) };

    if ( data.material->smoothShading )
        hitNormal = CalcHitNormal( data.hitPoint, data.triangle );

    const FVector3& surfaceNormal = data.material->smoothShading ? hitNormal : data.faceNormal;

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
            data.hitPoint + (surfaceNormal * m_scene.GetSettings().shadowBias) };
        Ray shadowRay{ offsetHitPoint, lightDir, -1, RayType::Shadow, false };

        // If there's something on the way to the light - leave the color as is.
        IntersectionData data = IntersectRay( shadowRay, lightDirLen );
        if ( data.filled )
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

    IntersectionData intersectData = IntersectRay( reflectionRay );
    Color pixelColor = Shade( reflectionRay, intersectData );
    pixelColor *= data.material->texture.albedo;

    return pixelColor;
}

float Render::CalcFresnelSchlickApprox( const float ior1, const float ior2, const float dotIN ) {
    // More info: https://en.wikipedia.org/wiki/Schlick%27s_approximation
    // More info: https://en.wikipedia.org/wiki/Fresnel_equations
    // Calculations at: https://webs.optics.arizona.edu/gsmith/FresnelCalculator.html
    float eta{ ior1 / ior2 };
    float cosIN = -dotIN; // cos(A)
    float R0 = std::pow( (ior1 - ior2) / (ior1 + ior2), 2.0f );
    // Use the cosine of the angle *inside* the denser material for the Fresnel term.
    // If ray was entering (original dotIN <= 0), use cosIN.
    // If ray was exiting (original dotIN > 0), use cosRnN.
    float cosRnN = std::sqrtf( 1.f - eta * eta * (1.f - (cosIN * cosIN)) );
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
        float sinRnN{ (std::sqrtf( 1.f - (cosIN * cosIN) ) * ior1) / ior2 }; //!? sin(B)
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
        IntersectionData refractData{ IntersectRay( refractionRay ) };
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
    IntersectionData reflectData{ IntersectRay( reflectionRay ) };
    Color reflectionColor = Shade( reflectionRay, reflectData );

    return reflectionColor * fresnel + refractionColor * (1.f - fresnel);
}

Color Render::Shade( const Ray& ray, const IntersectionData& data ) const {
    Color pixelColor{ m_scene.GetSettings().BGColor };

    if ( data.filled == false ) {
        // The camera ray didn't hit any objects - just the background.
        return pixelColor;
    } else if ( ray.pathDepth >= m_scene.GetSettings().pathDepth ) {
        return pixelColor; // or  Colors::Black
    } else if ( m_scene.GetRenderMode() == RenderMode::Barycentric ) {
        pixelColor = ShadeBary( data );
    } else if ( m_scene.GetRenderMode() == RenderMode::Normals ) {
        pixelColor = ShadeNormals( data );
    } else if ( m_scene.GetRenderMode() == RenderMode::ShadedNormals ) {
        pixelColor = ShadeDiffuse( data );
    } else if ( m_scene.GetRenderMode() == RenderMode::RandomTriangleColor ) {
        pixelColor = m_scene.GetTriangleColors()[data.triangle.colorIdx];
    } else if ( m_scene.GetRenderMode() == RenderMode::RandomMeshColor ) {
        pixelColor = data.material->texture.albedo;
    } else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Diffuse ) {
        pixelColor = ShadeDiffuse( data );
    } else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Reflective ) {
        pixelColor = ShadeReflective( ray, data );
    } else if ( m_scene.GetRenderMode() == RenderMode::Material
        && data.material->type == MaterialType::Refractive ) {
        pixelColor = ShadeRefractive( ray, data );
    } else {
        throw std::invalid_argument( "Unsupported render mode." );
    }

    return pixelColor;
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