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

FVector3 Camera::GenerateRay( const int x, const int y ) const {
    FVector2 ndcCoords = ray2NDC( x, y, m_imgPlane );
    FVector2 screenCoords = NDC2ScreenSpace( ndcCoords );
    FVector2 fixedAspectRatio = getFixedAspectRatio( screenCoords, m_imgPlane );
    FVector3 rayDirection = FVector3{ fixedAspectRatio, -1.f * m_imgPlane.distanceFromCamera };
    return ApplyRotation( rayDirection ).Normalize();
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
        float falloff = 4 * static_cast<float>( std::numbers::pi ) * lightDirLen * lightDirLen;

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
        ? triangle.color : intersectionData.currentMesh.m_material.albedo);

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

// TODO: Ask about shadow Bias value. Why 1e-4f creates problems?
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


Color Camera::GetTriangleIntersection(
    const FVector3& ray,
    const std::vector<PreparedMesh>& meshes,
    const Scene& scene,
    const unsigned reflectionDepth,
    const FVector3& rayOrigin
) const {
    Color pixelColor{ scene.GetSettings().BGColor };
    float closestIntersectionP{ std::numeric_limits<float>::max() };

    for ( const auto& mesh : meshes ) {

        for ( const Triangle& triangle : mesh.m_triangles ) {
            // rayProj = 0 -> Ray is parallel to surface - Ignore, it can't hit.
            // rayProj > 0 -> back-face
            // rayProj < 0 -> front-face
            float rayProj = ray.Dot( triangle.GetNormal() );
            if ( isGreaterEqualThan( rayProj, 0.f ) ) // Ignore back-face
                continue;

            float rayPlaneDist = (triangle.GetVert( 0 ).pos - rayOrigin)
                .Dot( triangle.GetNormal() );

            // Ray is not towards Triangle's plane
            if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
                continue;

            // Ray-to-Point scale factor for unit vector to reach the Point
            float rayPointDist = rayPlaneDist / rayProj;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = rayOrigin + (ray * rayPointDist);

            // Ignore intersection if a closer one to the Camera has already been found
            if ( rayPointDist > closestIntersectionP || isLessEqualThan( rayPointDist, 0.f ) )
                continue;

            // If the Plane intersection point is not inside the triangle - don't render it
            if ( !triangle.IsPointInside( intersectionPt ) )
                continue;

            closestIntersectionP = rayPointDist;

            if ( scene.GetRenderMode() == RenderMode::ObjectColor ) {
                if ( scene.GetSettings().colorMode == ColorMode::RandomTriangleColor )
                    pixelColor = triangle.color;
                else
                    pixelColor = mesh.m_material.albedo;
                continue;
            }

            if ( scene.GetRenderMode() == RenderMode::ShadedFlat ) {
                IntersectionData intersectionData( meshes, mesh, scene, intersectionPt );

                if ( mesh.m_material.type == MaterialType::Diffuse ) {
                    pixelColor = shade( intersectionData, triangle, nullptr );
                } else if ( mesh.m_material.type == MaterialType::Reflective ) {
                    if ( reflectionDepth > 0 ) {
                        // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
                        FVector3 reflectionDir = triangle.GetNormal() *
                            ray.Dot( triangle.GetNormal() ) * 2;
                        pixelColor = GetTriangleIntersection(
                            reflectionDir, meshes, scene, reflectionDepth - 1, intersectionPt );
                        pixelColor *= mesh.m_material.albedo;
                    } else {
                        pixelColor = Colors::Black;
                    }
                }
            }

            const FVector3 v0p = intersectionPt - triangle.GetVert( 0 ).pos;
            const FVector3 v0v2 = triangle.GetVert( 2 ).pos - triangle.GetVert( 0 ).pos;
            const FVector3 v0v1 = triangle.GetVert( 1 ).pos - triangle.GetVert( 0 ).pos;
            const float U = (v0p * v0v2).GetLength() / ( triangle.GetArea() * 2 );
            const float V = (v0v1 * v0p).GetLength() / ( triangle.GetArea() * 2 );
            const FVector3 hitNormal = triangle.GetVert( 1 ).normal * U
                + triangle.GetVert( 2 ).normal * V 
                + triangle.GetVert( 0 ).normal * (1 - U - V);

            if ( scene.GetRenderMode() == RenderMode::Barycentric ) {
                pixelColor = { U, V, 0.f }; // Display Barycentric Coordinates
                continue;
            }

            if ( scene.GetRenderMode() == RenderMode::ShadedSmooth ) {
                IntersectionData intersectionData( meshes, mesh, scene, intersectionPt );
                if ( mesh.m_material.type == MaterialType::Diffuse ) {
                    pixelColor = shade( intersectionData, triangle, &hitNormal );
                }
                else if ( mesh.m_material.type == MaterialType::Reflective ) {
                    if ( reflectionDepth > 0 ) {
                        // R = 2 * dot(A, N) * N = N * dot(A, N) * 2
                        FVector3 reflectionDir = ray - ( hitNormal * ray.Dot( hitNormal ) * 2 );
                        pixelColor = GetTriangleIntersection(
                            reflectionDir, meshes, scene, reflectionDepth - 1, intersectionPt );
                        pixelColor *= mesh.m_material.albedo;
                    } else {
                        pixelColor = Colors::Black;
                    }
                    //! Experiments for shadow and reflection at the same time. INCORRECT!
                    //pixelColor += shade( intersectionData, triangle, &hitNormal );
                    //pixelColor /= 2;
                }
            }
        }
    }

    return pixelColor;
}

ImagePlane::ImagePlane( int width, int height, float distFromCamera )
    : resolution{ static_cast<float>(width), static_cast<float>(height) },
    distanceFromCamera{ distFromCamera } {
}
