#include "Camera.h"
#include "Lights.h" // Light, PointLight
#include "Scene.h" // Scene
#include "SpaceConversions.h" // ray2NDC, NDC2ScreenSpace, getFixedAspectRatio
#include "utils.h" // isGreaterEqualThan

#include <algorithm> // round
#include <limits> // numeric_limits<float>::max
#include <numbers> // pi
#include <numeric> // gcd
#include <vector>

Camera::Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos ) {
    init();
}

Camera::~Camera() {
    for ( Obj* obj : m_children )
        delete obj;
}

FVector3 Camera::GetLocation() const {
    return m_position;
};

//? Currently is moving relatively? Check!!! Fix MoveRel if needed - moveAbs.
void Camera::Move( const FVector3& relPos ) {
    const FVector3 moveDirInWorldSpace{ relPos * m_orientation };

    std::vector<FVector3> childrenRelPos;
    childrenRelPos.reserve( m_children.size() );

    for ( Obj* child : m_children )
        childrenRelPos.push_back( m_position + child->GetLocation() );
        //childrenRelPos.push_back( child->getLocation() - m_location );

    m_position += moveDirInWorldSpace;
    for ( size_t i{}; i < m_children.size(); ++i )
        m_children[i]->Move( relPos + childrenRelPos[i] );
}

void Camera::init() {
    int gcd = std::gcd( static_cast<int>(m_imgPlane.resolution.x),
        static_cast<int>(m_imgPlane.resolution.y) );
    m_aspectRatio = { m_imgPlane.resolution.x / gcd, m_imgPlane.resolution.y / gcd };
}

Ray Camera::GenerateRay( const int x, const int y ) const {
    FVector2 ndcCoords = ray2NDC( x, y, m_imgPlane );
    FVector2 screenCoords = NDC2ScreenSpace( ndcCoords );
    FVector2 fixedAspectRatio = getFixedAspectRatio( screenCoords, m_imgPlane );
    FVector3 rayDirection = FVector3{ fixedAspectRatio, -1.f * m_imgPlane.distanceFromCamera };
    FVector3 finalDirection = ApplyRotation( rayDirection ).Normalize();
    return Ray( m_position, finalDirection, -1, RayType::Camera );
}

void Camera::Dolly( float val ) {
    Move( { 0.f, 0.f, val } );
}

void Camera::Truck( float val ) {
    Move( { val, 0.f, 0.f } );
}

void Camera::Pedestal( float val ) {
    Move( { 0.f, val, 0.f } );
}

void Camera::Pan( const float deg ) {
    Rotate( 0.f, deg, 0.f );
}

void Camera::Tilt( const float deg ) {
    Rotate( deg, 0.f, 0.f );
}

void Camera::Roll( const float deg ) {
    Rotate( 0.f, 0.f, deg );
}

void Camera::RotateAroundPoint( const FVector3& dist, const FVector3& angle ) {
    Move( -dist );
    Rotate( angle );
    Move( dist );
}

Color shade(
    const IntersectionData& intersectionData,
    const Triangle& triangle,
    const FVector3* hitNormal = nullptr
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
        if ( areEqual(cosLaw, 0.f) )
            continue;

        // Compute sphere area
        float falloff = 4 * std::numbers::pi_v<float> * lightDirLen * lightDirLen;

        // If there's something on the way to the light - leave the color as is and continue
        if ( Camera::IsInShadow(lightDir, intersectionData, surfaceNormal, lightDirLen ) )
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

    renderColor = (intersectionData.scene.GetSettings().colorMode == ColorMode::RandomTriangleColor
        ? triangle.color : intersectionData.currMeshMat.albedo);

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

bool Camera::IsInShadow(
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
    const FVector3 offsetHitPoint{ data.intersectionPt + (surfaceNormal * shadowBias ) };

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

FVector2 calculateBarycentricCoords(const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector3 v0p = intersectionPt - triangle.GetVert( 0 ).pos;
    const FVector3 v0v2 = triangle.GetVert( 2 ).pos - triangle.GetVert( 0 ).pos;
    const FVector3 v0v1 = triangle.GetVert( 1 ).pos - triangle.GetVert( 0 ).pos;
    const float U = (v0p * v0v2).GetLength() / (triangle.GetArea() * 2);
    const float V = (v0v1 * v0p).GetLength() / (triangle.GetArea() * 2);
    return { U, V };
}

FVector3 calculateHitNormal(const FVector3& intersectionPt, const Triangle& triangle ) {
    const FVector2 UV{ calculateBarycentricCoords( intersectionPt, triangle )};
    return triangle.GetVert( 1 ).normal * UV.x
        + triangle.GetVert( 2 ).normal * UV.y
        + triangle.GetVert( 0 ).normal * (1 - UV.x - UV.y);
}


Color Camera::GetTriangleIntersection(
    const Ray& ray,
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
            }
            else if ( scene.GetRenderMode() == RenderMode::Barycentric ) {
                const FVector2 UV = calculateBarycentricCoords( intersectionPt, triangle );
                pixelColor = { UV.x, UV.y, 0.f }; // Display Barycentric Coordinates
                continue;
            }

            // Shade Flat
            IntersectionData intersectionData(
                scene.GetPreparedMeshes(), mesh.m_material, scene, intersectionPt);

            switch ( mesh.m_material.type ) {
                case MaterialType::Diffuse: {
                    if ( mesh.m_material.smoothShading ) {
                        FVector3 hitNormal = calculateHitNormal( intersectionPt, triangle );
                        pixelColor = shade( intersectionData, triangle, &hitNormal );
                    }
                    else {
                        pixelColor = shade( intersectionData, triangle, nullptr );
                    }
                    break;
                }
                case MaterialType::Reflective: {
                    if ( reflectionDepth > 0 ) {
                        FVector3 useNormal{};
                        if ( mesh.m_material.smoothShading ) {
                            useNormal = calculateHitNormal( intersectionPt, triangle );
                        }
                        else {
                            useNormal = triangle.GetNormal();
                        }
                        // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
                        FVector3 reflectionDir = ray.direction - (
                            useNormal * ray.direction.Dot( useNormal ) * 2);
                        Ray reflectionRay{
                            intersectionPt,
                            reflectionDir,
                            scene.GetReflectionDepth(),
                            RayType::Reflective
                        };
                        pixelColor = GetTriangleIntersection(
                            reflectionRay,
                            scene,
                            reflectionDepth - 1
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

ImagePlane::ImagePlane( int width, int height, float distFromCamera )
    : resolution{ static_cast<float>(width), static_cast<float>(height) },
    distanceFromCamera{ distFromCamera } {
}
