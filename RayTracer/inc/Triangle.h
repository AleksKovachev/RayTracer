#ifndef SHAPES_H
#define SHAPES_H

#include <optional>

#include "Bases.h"   // Obj, Color
#include "Vectors.h"

constexpr int vertsInTriangle{ 3 };

// Triangle class with position vertices
class Triangle : public Obj {
public:
    Color color;

    Triangle() { init( {}, {}, {} ); }
    Triangle( const FVector3& vert0, const FVector3& vert1, const FVector3& vert2 );

    // Calculates Edges, Normal Vector and Area
    void init( const FVector3& v0, const FVector3& v1, const FVector3& v2 );

    virtual FVector3 GetLocation() const override {
        return m_position;
    }

    virtual void Move( const FVector3& vec ) override;

    // Returns Normal Vector
    FVector3 GetNormal() const {
        return normal;
    }

    // Returns Triangle Area
    float GetArea() const {
        return area;
    }

    // std::optional<FVector3> GetVert( const int vertIdx ) const;

    // Get vertex at index
    FVector3 GetVert( const int vertIdx ) const;

    // Change vertex position and recalculate Normal Vector and Area
    void SetVert( const FVector3& point, const int vertIdx );
    std::optional<FVector3> GetEdge( const int edgeIdx ) const;

    // Checks if a given point is inside the triangle
    bool IsPointInside( const FVector3& point ) const;
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    FVector3 verts[vertsInTriangle];
    FVector3 edges[vertsInTriangle];
    FVector3 normal;
    float area;

    void CalculateNormal();
    void CalculateArea();
};

#endif // SHAPES_H

/*
* Triangle area 3 sides:
* s = (a + b + c) / 2 -> semi perimeter
* area = sqrt(s * (s - a) * (s - b) * (s - c))
*/