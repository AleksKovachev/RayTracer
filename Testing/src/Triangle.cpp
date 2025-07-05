#include "Triangle.h"
#include "utils.h"

Triangle::Triangle( const FVector3& vert0, const FVector3& vert1, const FVector3& vert2 )
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

FVector3 Triangle::GetNormal() const {
    return normal;
}

float Triangle::GetArea() const {
    return area;
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

bool Triangle::IsPointInside( const FVector3& point ) const {
    if ( isLessThan( (edges[0] * (point - verts[0])).Dot( normal ), 0.f ) )
        return false;
    if ( isLessThan( (edges[1] * (point - verts[1])).Dot( normal ), 0.f ) )
        return false;
    if ( isLessThan( (edges[2] * (point - verts[2])).Dot( normal ), 0.f ) )
        return false;

    return true;
}
