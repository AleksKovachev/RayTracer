#ifndef SHAPES_H
#define SHAPES_H

#include "Colors.h" // Color
#include "Vectors.h" // FVector3

constexpr int vertsInTriangle{ 3 };

struct Vertex {
    FVector3 pos;
    FVector3 normal;
    int origIdx;
};


// Triangle class with position vertices
class Triangle {
public:
    Color color;

    Triangle() { init( {}, {}, {} ); }
    Triangle( const FVector3& vert0, const FVector3& vert1, const FVector3& vert2 );
    Triangle( const Vertex& vert0, const Vertex& vert1, const Vertex& vert2 );

    // Calculates Edges, Normal Vector and Area
    void init( const FVector3& v0, const FVector3& v1, const FVector3& v2 );

    // Returns Normal Vector
    FVector3 GetNormal() const;

    // Returns Triangle Area
    float GetArea() const;

    // std::optional<FVector3> GetVert( const int vertIdx ) const;

    // Get vertex at index
    Vertex GetVert( const int vertIdx ) const;

    // Assigns value to the normal vector of the vertex with the provided index
    void SetVertexNormal( const int vertIdx, const FVector3& val );

    // Checks if a given point is inside the triangle
    bool IsPointInside( const FVector3& point ) const;
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    Vertex verts[vertsInTriangle];
    FVector3 edges[vertsInTriangle];
    FVector3 normal;
    float area;

    void CalculateNormal();
    void CalculateArea();
};

#endif // SHAPES_H
