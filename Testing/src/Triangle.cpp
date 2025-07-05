#include "Triangle.h"

Triangle::Triangle( const FVector3& vert0, const FVector3& vert1, const FVector3& vert2 )
    : verts{ vert0, vert1, vert2 } {
    init( vert0, vert1, vert2 );
}

void Triangle::init( const FVector3& v0, const FVector3& v1, const FVector3& v2 ) {
    edges[0] = v1 - v0;
    edges[1] = v2 - v1;
    edges[2] = v0 - v2;
    CalculateNormal();
}

FVector3 Triangle::GetNormal() const {
    return normal;
}

void Triangle::CalculateNormal() {
    normal = (edges[0] * (verts[2] - verts[0])).normalize();
}

FVector3 Triangle::GetVert( const int vertIdx ) const {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        return verts[vertIdx];
    std::cerr << "Wrong Vertex Index" << std::endl;
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
