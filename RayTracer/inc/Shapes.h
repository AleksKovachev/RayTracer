#ifndef SHAPES_H
#define SHAPES_H

#include <optional>

#include "Bases.h"
#include "Vectors.h"

constexpr int vertsInTriangle{ 3 };

// Triangle class with position vertices
class Triangle : public Obj {
public:
    Triangle() = default;
    Triangle( FVector3 vert0, FVector3 vert1, FVector3 vert2 );

    void init( const FVector3& v0, const FVector3& v1, const FVector3& v2 );

    FVector3 getNormalVector() const {
        return normal;
    }
    double getArea() const {
        return area;
    }

    std::optional<FVector3> getVert( const int vertIdx ) const; 
    // Recalculate edges upon vertex change
    void setVert( const FVector3& point, const int vertIdx );
    std::optional<FVector3> getEdge( const int edgeIdx ) const;
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    FVector3 verts[vertsInTriangle];
    FVector3 edges[vertsInTriangle];
    FVector3 normal;
    double area;
};

#endif // SHAPES_H

/*
* Triangle area 3 sides:
* s = (a + b + c) / 2 -> semi perimeter
* area = sqrt(s * (s - a) * (s - b) * (s - c))
*/