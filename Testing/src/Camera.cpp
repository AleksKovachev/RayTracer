#include "Camera.h"
#include "Lights.h" // Light
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
    return ApplyRotation( rayDirection ).normalize();
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

Color shade( const IntersectionData& intersectionData, const Triangle& triangle ) {
    float R{};
    float G{};
    float B{};
    float falloff{};
    Color pixelColor{};

    for ( const Light* light : intersectionData.scene.GetLights() ) {
        //! If Scene lights get more types, replace static_cast with dynamic to check light type.
        const PointLight* ptLight = static_cast<const PointLight*>(light);
        FVector3 lightDir = ptLight->GetPosition() - intersectionData.intersectionPt;

        // If there's something on the way to the light - leave the color as is and continue
        if ( Camera::IsInShadow(lightDir, intersectionData, triangle.GetNormal() ) )
            continue;

        float lightDirLen = lightDir.GetLength();
        // Avoid 0-division after falloff calculation
        if ( areEqual( lightDirLen, 0.f ) )
            continue;

        lightDir.NormalizeInPlace();
        // Negative numbers are 0 in color. Positive numbers above 1 are clipped
        float cosLaw = std::max( 0.f, lightDir.Dot( triangle.GetNormal() ) );
        if ( areEqual(cosLaw, 0.f) )
            continue;

        falloff = 4 * static_cast<float>( std::numbers::pi ) * lightDirLen * lightDirLen;
        cosLaw *= ptLight->GetIntensity() / falloff;
        //! Clamp here to make 1.0 the max intensity!
        // cosLaw = std::min( 1.f, cosLaw );

        R += cosLaw;
    }

    if ( areEqual( R, 0.f ) )
        return Colors::Black;

    G = B = R;

    // multiply by normalized albedo
    const int& maxComp = intersectionData.scene.GetSettings().maxColorComp;
    R *= static_cast<float>(triangle.color.r) / maxComp;
    G *= static_cast<float>(triangle.color.g) / maxComp;
    B *= static_cast<float>(triangle.color.b) / maxComp;

    //! Clamp here to make everything > 1.0 clip back to 1.0!
    pixelColor.r = static_cast<int>(round( std::min( 1.f, R ) * maxComp ));
    pixelColor.g = static_cast<int>(round( std::min( 1.f, G ) * maxComp ));
    pixelColor.b = static_cast<int>(round( std::min( 1.f, B ) * maxComp ));

    return pixelColor;
}

//TODO: Check why removing the epsilon results in complete darkness instead of broken lighting. Ask!!!
//TODO: Check if the rayPointDist < shadowBias... row is correct
bool Camera::IsInShadow( const FVector3& rayDir, const IntersectionData& data, const FVector3& triN ) {
    // Define a small epsilon to avoid self-intersection artifacts
    // Often 1e-3 - 1e-5 is used for ray origins.
    constexpr float shadowBias{ 1e-4f }; // Smaller value if scene scale is tiny

    /* Offset the hitPoint slightly along the normal to avoid self - intersection
     * Another common technique is to check rayPointDist > EPSILON */
    const FVector3 offsetHitPoint{ data.intersectionPt + triN * shadowBias };

    for ( const PreparedMesh& mesh : data.meshes ) {
        for ( const Triangle& triangle : mesh.m_triangles ) {
            // If Ray is parallel - Ignore, it can't be hit.
            float rayProj = rayDir.Dot( triangle.GetNormal() );
            if ( areEqual( rayProj, 0.f ) )
                continue;

            // If rayProj > 0, ray is pointing towards triangle back face.
            // If rayProj < 0, ray is pointing towards triangle front face.

            float rayPlaneDist = (triangle.GetVert( 0 ) - offsetHitPoint).Dot( triangle.GetNormal() );
            float rayPointDist = rayPlaneDist / rayProj; // Ray-to-Point scale factor

            /* Check if: Ray hits behind the hitPoint(negative rayPointDist)
             *           Ray hits at or very near the hitPoint (self-intersection)
             *           Ray hits beyond the light source */

            if ( rayPointDist < shadowBias )
                continue; // This intersection is not valid for shadow casting

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = offsetHitPoint + (rayDir * rayPointDist);

            if ( triangle.IsPointInside( intersectionPt ) )
                return true;
        }
    }
    return false;
}


Color Camera::GetTriangleIntersection(
    const FVector3& ray,
    const std::vector<PreparedMesh>& meshes,
    const Scene& scene
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

            float rayPlaneDist = (triangle.GetVert( 0 ) - m_position)
                .Dot( triangle.GetNormal() );

            // Ray is not towards Triangle's plane
            if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
                continue;

            // Ray-to-Point scale factor for unit vector to reach the Point
            float rayPointDist = rayPlaneDist / rayProj;

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = m_position + (ray * rayPointDist);

            // Ignore intersection if a closer one to the Camera has already been found
            if ( rayPointDist > closestIntersectionP )
                continue;

            if ( triangle.IsPointInside( intersectionPt ) ) {
                closestIntersectionP = rayPointDist;
                //pixelColor = triangle.color;
                IntersectionData intersectionData( meshes, mesh, scene, intersectionPt );

                pixelColor = shade( intersectionData, triangle );
            }
        }
    }

    return pixelColor;
}

ImagePlane::ImagePlane( int width, int height, float distFromCamera )
    : resolution{ static_cast<float>(width), static_cast<float>(height) },
    distanceFromCamera{ distFromCamera } {
}
