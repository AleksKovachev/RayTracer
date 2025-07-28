#include "Camera.h"
#include "SpaceConversions.h"
#include "Vectors.h"

#include <vector>

Camera::Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos )
    : Obj( pos ), m_imgPlane{ imgPlane }, m_direction{ direction } {
    m_children.reserve( 1 );
    m_children.push_back( m_imgPlane );
}

void Camera::move( const FVector3& abs_pos ) {
    std::vector<FVector3> childrenRelPos;
    childrenRelPos.reserve( m_children.size() );

    for ( Obj& child : m_children )
        childrenRelPos.push_back( m_position + child.m_position );

    m_position = abs_pos;

    for ( size_t i{}; i < m_children.size(); ++i )
        m_children[i].m_position = abs_pos + childrenRelPos[i];
}

void Camera::moveRel( const FVector3& rel_pos ) {
    m_position += rel_pos;

    for ( Obj& child : m_children ) {
        child.m_position += rel_pos;
    }
}

FVector3 Camera::generateRay( const int x, const int y ) const {
    FVector2 ndcCoords{};
    FVector2 screenCoords{};
    FVector2 fixedAspectRatio{};
    ndcCoords = ray2NDC( x, y, m_imgPlane );
    screenCoords = NDC2ScreenSpace( ndcCoords );
    fixedAspectRatio = getFixedAspectRatio( screenCoords, m_imgPlane );
    return FVector3{ fixedAspectRatio, m_imgPlane.m_position.z }.normalize();
}

Color Camera::getTriangleIntersection(
    const FVector3 ray,
    const std::vector<Triangle>& triangles,
    const Camera& camera
) const {
    Color blackBG{ 0, 0, 0 };
    Color& pixelColor{ blackBG };
    double rayProj{};
    double rayPlaneDist{};
    double rayPointDist{}; // Ray-to-Point scale factor
    FVector3 intersectionPt{};
    double intersectionPLen{ std::numeric_limits<double>::max() };
    double closestIntersectionP{ std::numeric_limits<double>::max() };

    for ( const Triangle& triangle : triangles ) {
        // Ignore if Ray is parallel or hits triangle back.
        rayProj = ray.dot( triangle.getNormal() );
        if ( rayProj >= 0.0 )
            continue;
        rayPlaneDist = triangle.getVert( 0 ).dot( triangle.getNormal() );

        // Ray is not towards Triangle's plane
        if ( rayPlaneDist >= 0 )
            continue; // rayPlaneDist > 0 -> Back-face culling

        rayPointDist = (abs( rayPlaneDist ) * ray.getLength()) / abs( rayProj ); // t

        // Ray parametric equation - represent points on a line going through a Ray.
        intersectionPt = camera.m_position + (ray * rayPointDist);
        intersectionPLen = intersectionPt.getLength();

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
