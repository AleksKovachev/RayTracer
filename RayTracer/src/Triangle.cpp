#include "Triangle.h"

Triangle::Triangle( FVector3 vert0, FVector3 vert1, FVector3 vert2 )
    : verts{ vert0, vert1, vert2 } {
    init( vert0, vert1, vert2 );
}

void Triangle::init( const FVector3& v0, const FVector3& v1, const FVector3& v2 ) {
    edges[0] = v1 - v0;
    edges[1] = v2 - v1;
    edges[2] = v0 - v2;
    CalculateNormal();
    CalculateArea();
}

void Triangle::Move( const FVector3& pos ) {
    const FVector3 moveDirInWorldSpace{ pos * m_orientation };

    // Update all vertices locations
    for ( int i{}; i < vertsInTriangle; ++i ) {
        verts[i] = verts[i] - m_position + moveDirInWorldSpace;
    }
    m_position += moveDirInWorldSpace;
    init( verts[0], verts[1], verts[2] );
}

void Triangle::CalculateNormal() {
    normal = (edges[0] * (verts[2] - verts[0])).normalize();
}

void Triangle::CalculateArea() {
    area = (edges[0] * edges[1]).GetLength() / 2.f;
}

FVector3 Triangle::GetVert( const int vertIdx ) const {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        return verts[vertIdx];
    std::cerr << "Wrong Vertex Index" << std::endl;
}

// Valid but unused at the moment as it complicates code
//std::optional<FVector3> Triangle::GetVert( const int vertIdx ) const {
//    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
//        return verts[vertIdx];
//    std::cerr << "Wrong Vertex Index" << std::endl;
//    return std::nullopt;
//}

void Triangle::SetVert( const FVector3& point, const int vertIdx ) {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle ) {
        verts[vertIdx] = point;
        init( verts[0], verts[1], verts[2] );
    }
    std::cerr << "Wrong Vertex Index" << std::endl;
}

std::optional<FVector3> Triangle::GetEdge( const int edgeIdx ) const {
    if ( 0 <= edgeIdx && edgeIdx < vertsInTriangle )
        return edges[edgeIdx];
    std::cerr << "Wrong Edge Index" << std::endl;
    return std::nullopt;
}

bool Triangle::IsPointInside( const FVector3& point ) const {
    if ( (edges[0] * (point - verts[0])).Dot( normal ) <= 0.f )
        return false;
    if ( (edges[1] * (point - verts[1])).Dot( normal ) <= 0.f )
        return false;
    if ( (edges[2] * (point - verts[2])).Dot( normal ) <= 0.f )
        return false;

    return true;
}
