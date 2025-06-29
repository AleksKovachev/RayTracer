#include "Triangle.h"

Triangle::Triangle( FVector3 vert0, FVector3 vert1, FVector3 vert2 )
    : verts{ vert0, vert1, vert2 } {
    init( vert0, vert1, vert2 );
}

void Triangle::init( const FVector3& v0, const FVector3& v1, const FVector3& v2 ) {
    edges[0] = v1 - v0;
    edges[1] = v2 - v1;
    edges[2] = v0 - v2;
    calculateNormal();
    calculateArea();
}

void Triangle::move( const FVector3& vec ) {
    // Update all vertices locations
    for ( int i{}; i < vertsInTriangle; ++i ) {
        verts[i] = verts[i] - m_position + vec;
    }
    m_position = vec;
    init( verts[0], verts[1], verts[2] );
}

void Triangle::calculateNormal() {
    normal = (edges[0] * (verts[2] - verts[0])).normalize();
}

void Triangle::calculateArea() {
    area = (edges[0] * edges[1]).getLength() / 2.f;
}

FVector3 Triangle::getVert( const int vertIdx ) const {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        return verts[vertIdx];
    std::cerr << "Wrong Vertex Index" << std::endl;
}

// Valid but unused at the moment as it complicates code
//std::optional<FVector3> Triangle::getVert( const int vertIdx ) const {
//    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
//        return verts[vertIdx];
//    std::cerr << "Wrong Vertex Index" << std::endl;
//    return std::nullopt;
//}

void Triangle::setVert( const FVector3& point, const int vertIdx ) {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle ) {
        verts[vertIdx] = point;
        init( verts[0], verts[1], verts[2] );
    }
    std::cerr << "Wrong Vertex Index" << std::endl;
}

std::optional<FVector3> Triangle::getEdge( const int edgeIdx ) const {
    if ( 0 <= edgeIdx && edgeIdx < vertsInTriangle )
        return edges[edgeIdx];
    std::cerr << "Wrong Edge Index" << std::endl;
    return std::nullopt;
}

bool Triangle::isPointInside( const FVector3& point ) const {
    if ( (edges[0] * (point - verts[0])).dot( normal ) <= 0.f )
        return false;
    if ( (edges[1] * (point - verts[1])).dot( normal ) <= 0.f )
        return false;
    if ( (edges[2] * (point - verts[2])).dot( normal ) <= 0.f )
        return false;

    return true;
}
