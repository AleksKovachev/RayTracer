#include "Camera.h"
#include "SpaceConversions.h"
#include "Vectors.h"
#include "utils.h"

#include <cmath>
#include <numeric>
#include <vector>

void Camera::move( const FVector3& pos ) {
    std::vector<FVector3> childrenRelPos;
    childrenRelPos.reserve( m_children.size() );

    for ( Obj* child : m_children )
        childrenRelPos.push_back( m_position + child->getLocation() );
        //childrenRelPos.push_back( child->getLocation() - m_location );

    m_position = pos;
    for ( size_t i{}; i < m_children.size(); ++i )
        m_children[i]->move( pos + childrenRelPos[i] );
}

void Camera::init() {
    m_children.reserve( 1 );
    m_children.push_back( &m_imgPlane );
    m_distToPlane = (m_position - m_imgPlane.getLocation()).getLength();

    int gcd = std::gcd( static_cast<int>(m_imgPlane.resolution.x), static_cast<int>(m_imgPlane.resolution.y) );
    m_aspectRatio = { m_imgPlane.resolution.x / gcd, m_imgPlane.resolution.y / gcd };
}

void Camera::moveRel( const FVector3& rel_pos ) {
    m_position += rel_pos;

    for ( Obj* child : m_children )
        child->move( child->getLocation() + rel_pos );
}

FVector3 Camera::generateRay( const int x, const int y ) const {
    FVector2 ndcCoords = ray2NDC( x, y, m_imgPlane );
    FVector2 screenCoords = NDC2ScreenSpace( ndcCoords );
    FVector2 fixedAspectRatio = getFixedAspectRatio( screenCoords, m_imgPlane );
    FVector3 rayDirection = FVector3{ fixedAspectRatio, m_imgPlane.getLocation().z }.normalize();
    return ApplyRotation( rayDirection );
}

void Camera::dolly( float val ) {
    move( { m_position.x, m_position.y, val } );
}

void Camera::truck( float val ) {
    move( { val, m_position.y, m_position.z } );
}

void Camera::pedestal( float val ) {
    move( { m_position.x, val, m_position.z } );
}

Color Camera::getTriangleIntersection(
    const FVector3 ray,
    const std::vector<Triangle>& triangles,
    const Camera& camera
) const {
    Color blackBG{ 0, 0, 0 };
    Color& pixelColor{ blackBG };
    float closestIntersectionP{ std::numeric_limits<float>::max() };

    for ( const Triangle& triangle : triangles ) {
        // Ignore if Ray is parallel or hits triangle back.
        float rayProj = ray.dot( triangle.getNormal() );
        if ( isGreaterEqualThan(rayProj, 0.f ) )
            continue;

        float rayPlaneDist = (triangle.getVert( 0 ) - camera.m_position).dot( triangle.getNormal() );
        // Ray is not towards Triangle's plane
        if ( isGreaterEqualThan(rayPlaneDist, 0.f) )
            continue; // rayPlaneDist > 0 -> Back-face culling

        /* Check if rayPlaneDist direction is needed or length
        * abs(rayPlaneDist) should be multiplied by Ray length, but is
        * ommited as Rays are unit Vectors so their length is always 1 */
        float rayPointDist = rayPlaneDist / rayProj; // Ray-to-Point scale factor
        //? Creates floating triangles if parallel check is ==, ray-towards-tri is >=.
        //? float rayPointDist = abs( rayPlaneDist ) / abs( rayProj );

        // Ray parametric equation - represent points on a line going through a Ray.
        FVector3 intersectionPt = camera.m_position + (ray * rayPointDist);
        float intersectionPLen = intersectionPt.getLength();

        // Ignore intersection if a closer one to the Camera has already been found
        if ( intersectionPLen > closestIntersectionP )
            continue;

        if ( triangle.isPointInside( intersectionPt ) ) {
            closestIntersectionP = intersectionPLen;
            pixelColor = triangle.color;
        }
    }

    return pixelColor;
}
