#ifndef SHAPES_H
#define SHAPES_H

#include <optional>

#include "Bases.h"
#include "Vectors.h"

constexpr int vertsInTriangle{ 3 };

// Triangle class with position vertices
class Triangle : public Obj {
public:
    Color color;

    Triangle() = default;
    Triangle( FVector3 vert0, FVector3 vert1, FVector3 vert2 );

    // Calculates Edges, Normal Vector and Area
    void init( const FVector3& v0, const FVector3& v1, const FVector3& v2 );

    virtual FVector3 getLocation() const override {
        return m_location;
    }

    virtual void move( const FVector3& vec ) override;

    // Returns Normal Vector
    FVector3 getNormal() const {
        return normal;
    }

    // Returns Triangle Area
    float getArea() const {
        return area;
    }

    // std::optional<FVector3> getVert( const int vertIdx ) const;

    // Get vertex at index
    FVector3 getVert( const int vertIdx ) const;

    // Change vertex position and recalculate Normal Vector and Area
    void setVert( const FVector3& point, const int vertIdx );
    std::optional<FVector3> getEdge( const int edgeIdx ) const;

    // Checks if a given point is inside the triangle
    bool isPointInside( const FVector3& point ) const;
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    FVector3 verts[vertsInTriangle];
    FVector3 edges[vertsInTriangle];
    FVector3 normal;
    float area;

    void calculateNormal();
    void calculateArea();
};

#endif // SHAPES_H

/*
* Triangle area 3 sides:
* s = (a + b + c) / 2 -> semi perimeter
* area = sqrt(s * (s - a) * (s - b) * (s - c))
*/