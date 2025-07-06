#include "Triangle.h"
#include "utils.h"

Triangle::Triangle( const FVector3& vert0, const FVector3& vert1, const FVector3& vert2 ) {
    verts[0].pos = vert0;
    verts[1].pos = vert1;
    verts[2].pos = vert2;
    init( vert0, vert1, vert2 );
}
Triangle::Triangle( const Vertex& vert0, const Vertex& vert1, const Vertex& vert2 ) {
    verts[0] = vert0;
    verts[1] = vert1;
    verts[2] = vert2;
    init( vert0.pos, vert1.pos, vert2.pos );
}
;

void Triangle::init( const FVector3& v0, const FVector3& v1, const FVector3& v2 ) {
    edges[0] = v1 - v0;
    edges[1] = v2 - v1;
    edges[2] = v0 - v2;
    CalculateNormal();
    CalculateArea();
    verts[0].normal = {};
    verts[1].normal = {};
    verts[2].normal = {};
}

FVector3 Triangle::GetNormal() const {
    return normal;
}

float Triangle::GetArea() const {
    return area;
}

void Triangle::CalculateNormal() {
    normal = (edges[0] * (verts[2].pos - verts[0].pos)).Normalize();
}

void Triangle::CalculateArea() {
    area = (edges[0] * edges[1]).GetLength() / 2.f;
}

Vertex Triangle::GetVert( const int vertIdx ) const {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle )
        return verts[vertIdx];
    std::cerr << "Wrong Vertex Index" << std::endl;
}

void Triangle::SetVertexNormal( const int vertIdx, const FVector3& val ) {
    if ( 0 <= vertIdx && vertIdx < vertsInTriangle ) {
        verts[vertIdx].normal = val;
        return;
    }
    std::cerr << "Wrong Vertex Index" << std::endl;
}

bool Triangle::IsPointInside( const FVector3& point ) const {
    if ( isLessThan( (edges[0] * (point - verts[0].pos)).Dot( normal ), 0.f ) )
        return false;
    if ( isLessThan( (edges[1] * (point - verts[1].pos)).Dot( normal ), 0.f ) )
        return false;
    if ( isLessThan( (edges[2] * (point - verts[2].pos)).Dot( normal ), 0.f ) )
        return false;

    return true;
}
