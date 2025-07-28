#include "Shapes.h"

Triangle::Triangle( FVector3 vert0, FVector3 vert1, FVector3 vert2 )
    : verts{ vert0, vert1, vert2 } {
    init( vert0, vert1, vert2 );
}

void Triangle::init( const FVector3& v0, const FVector3& v1, const FVector3& v2 ) {
    edges[0] = v1 - v0;
    edges[1] = v2 - v1;
    edges[2] = v0 - v2;
    normal = (edges[0] * (v2 - v0)).normalize();
    area = (edges[0] * edges[1]).getLength() / 2;
}

std::optional<FVector3> Triangle::getVert( const int vertIdx ) const {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        return verts[vertIdx];
    std::cerr << "Wrong Vertex Index" << std::endl;
    return std::nullopt;
}

// Recalculate edges upon vertex change
void Triangle::setVert( const FVector3& point, const int vertIdx ) {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        verts[vertIdx] = point;
    std::cerr << "Wrong Vertex Index" << std::endl;
}

std::optional<FVector3> Triangle::getEdge( const int edgeIdx ) const {
    if ( 0 <= edgeIdx && edgeIdx < vertsInTriangle )
        return edges[edgeIdx];
    std::cerr << "Wrong Edge Index" << std::endl;
    return std::nullopt;
}
