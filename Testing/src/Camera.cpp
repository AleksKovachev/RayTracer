#include "Bases.h" // Color
#include "Camera.h"
#include "Lights.h" // Light
#include "Scene.h" // Scene
#include "SpaceConversions.h" // ray2NDC, NDC2ScreenSpace, getFixedAspectRatio
#include "Triangle.h"
#include "utils.h" // isGreaterEqualThan
#include "Vectors.h" // FVector3

#include <algorithm>
#include <cmath>
#include <numbers>
#include <numeric>
#include <vector>

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
    int gcd = std::gcd( static_cast<int>(m_imgPlane.resolution.x), static_cast<int>(m_imgPlane.resolution.y) );
    m_aspectRatio = { m_imgPlane.resolution.x / gcd, m_imgPlane.resolution.y / gcd };
}

void Camera::MoveAbs( const FVector3& absPos ) {
    m_position = absPos;

    // TODO Handle children movement
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

Color shade( const Scene& scene, const FVector3& intersectionPt, const FVector3& triNormal, const Mesh& mesh ) {
    float R{};
    float G{};
    float B{};
    float falloff{};
    int lightsCounter{};
    Color pixelColor{};

    for ( const Light* light : scene.GetLights() ) {
        const PointLight* ptLight = static_cast<const PointLight*>(light);

        FVector3 lightDir = ptLight->GetPosition() - intersectionPt;
        float lightDirLen = lightDir.GetLength();
        lightDir.NormalizeInPlace();
        // Negative numbers are 0 in color. Positive numbers above 1 are clipped
        float cosLaw =  std::max( 0.f, lightDir.Dot( triNormal ) );
        falloff += 4 * static_cast<float>( std::numbers::pi ) * lightDirLen * lightDirLen;
        cosLaw *= ptLight->GetIntensity() / falloff;
        //! Clamp here to make 1.0 the max intensity!
        // cosLaw = std::min( 1.f, cosLaw );

        R += cosLaw;

        lightsCounter++;
    }

    if ( lightsCounter == 0 )
        return { 0.f, 0.f, 0.f };

    R /= lightsCounter;
    G = B = R;

    // multiply by normalized albedo
    const int& maxComp = scene.GetSettings().maxColorComp;
    R *= static_cast<float>(mesh.albedo.r) / maxComp;
    G *= static_cast<float>(mesh.albedo.g) / maxComp;
    B *= static_cast<float>(mesh.albedo.b) / maxComp;

    //! Clamp here to make everything > 1.0 clip back to 1.0!
    pixelColor.r = static_cast<int>(round( R * maxComp ));
    pixelColor.g = static_cast<int>(round( G * maxComp ));
    pixelColor.b = static_cast<int>(round( B * maxComp ));
    pixelColor.Clamp();

    //!? Keep colors as if light was 1.0. Remove Clamp to use.
    //pixelColor.ConvertToRange( static_cast<unsigned>( log2( maxComp + 1 ) ) );

    return pixelColor;
}


Color Camera::GetTriangleIntersection(
    const FVector3& ray,
    const MeshInfo& meshes,
    const Scene& scene
) const {
    Color pixelColor{ scene.GetSettings().BGColor };
    int colorCounter{};
    float closestIntersectionP{ std::numeric_limits<float>::max() };

    for ( const auto& mesh : meshes ) {

        for ( const Triangle& triangle : mesh.second ) {
            // Ignore if Ray is parallel or hits triangle back.
            float rayProj = ray.Dot( triangle.GetNormal() );
            if ( areEqual( rayProj, 0.f ) )
                continue;

            float rayPlaneDist = (triangle.GetVert( 0 ) - m_position).Dot( triangle.GetNormal() );

            //! Skip backface check so backface triangles can cast shadows
            // Ray is not towards Triangle's plane
            //if ( isGreaterEqualThan( rayPlaneDist, 0.f ) )
            //    continue; // rayPlaneDist > 0 -> Back-face culling

            /* Check if rayPlaneDist direction is needed or length
            * abs(rayPlaneDist) should be multiplied by Ray length, but is
            * ommited as Rays are unit Vectors so their length is always 1 */
            float rayPointDist = rayPlaneDist / rayProj; // Ray-to-Point scale factor
            //? Creates floating triangles if parallel check is ==, ray-towards-tri is >=.
            //? float rayPointDist = abs( rayPlaneDist ) / abs( rayProj );

            // Ray parametric equation - represent points on a line going through a Ray.
            FVector3 intersectionPt = m_position + (ray * rayPointDist);
            float intersectionPLen = intersectionPt.GetLength();

            // Ignore intersection if a closer one to the Camera has already been found
            if ( intersectionPLen > closestIntersectionP )
                continue;

            if ( triangle.IsPointInside( intersectionPt ) ) {
                closestIntersectionP = intersectionPLen;
                //pixelColor = triangle.color;
                Color shadedColor = shade( scene, intersectionPt, triangle.GetNormal(), *mesh.first );
                if ( colorCounter == 0 )
                    pixelColor = shadedColor;
                else
                    pixelColor += shadedColor;
                colorCounter++;
            }
        }
    }

    pixelColor = (colorCounter > 0) ? pixelColor / colorCounter : pixelColor;

    return pixelColor;
}
