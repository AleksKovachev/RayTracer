#ifndef SHAPES_H
#define SHAPES_H

#include "Colors.h" // Color
#include "Vectors.h" // FVector3

constexpr int vertsInTriangle{ 3 };

struct Vertex {
    FVector3 pos{};
    FVector3 normal{}; // Normal vector of the vertex.
    FVector3 UVCoords{};
    int origIdx{}; // Index of this vertex in the scene file.
};


// Triangle class with position vertices.
class Triangle {
public:
    unsigned matIdx;
    unsigned overrideMatIdx;

    Color color;

    Triangle();
    // @param[in] vert0: The first triplet vertex position.
    // @param[in] vert1: The next counter-cloackwise vertex position.
    // @param[in] vet2: The last remaining vertex position.
    Triangle( const FVector3&, const FVector3&, const FVector3& );
    // @param[in] vert0: The first triplet vertex.
    // @param[in] vert1: The next counter-cloackwise vertex.
    // @param[in] vet2: The last remaining vertex.
    Triangle( const Vertex&, const Vertex&, const Vertex& );

    // Calculates Edges, Normal Vector and Area.
    // @param[in] vert0: The first triplet vertex position.
    // @param[in] vert1: The next counter-cloackwise vertex position.
    // @param[in] vet2: The last remaining vertex position.
    void init( const FVector3&, const FVector3&, const FVector3& );

    // Gets the Normal Vector.
    // @return The normal vector of the trianlge.
    FVector3 GetNormal() const;

    // Gets the Area
    // @return The area of the triangle.
    float GetArea() const;

    // Get vertex at index.
    // @param[in] vertIdx: The relative to the triangle index of the vertex to get.
    // @return The requested vertex.
    Vertex GetVert( const unsigned ) const;

    // Assigns value to the normal vector of the vertex with the provided index
    // @param[in] vertIdx: The relative to the triangle vertex index to set.
    // @param[in] val: The vertex normal vector value to set.
    void SetVertexNormal( const unsigned, const FVector3& );

    // Checks if a given point is inside the triangle
    // @param[in] point: 3D world space coordinates of the point to check.
    // @return If the point is inside this triangle.
    bool IsPointInside( const FVector3& ) const;
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    Vertex verts[vertsInTriangle];
    FVector3 edges[vertsInTriangle];
    FVector3 normal;
    float area;

    // Calculates the normal vector of this triangle.
    void CalculateNormal();
    // Calculates the area of this triangle.
    void CalculateArea();
};

#endif // SHAPES_H
