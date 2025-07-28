#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "Bases.h"
#include "Triangle.h"

// Camera Image Plane object with a given resolution and position.
struct ImagePlane : Obj {
    FVector2 resolution;
    ImagePlane( int width = 1920, int height = 1080, Vector3 pos = { 0, 0, -1 } )
        : resolution{ static_cast<double>(width), static_cast<double>(height) }, Obj( pos ) {
    }
};

/* Camera class with an Image Plane, position, orientation and children. Any child object
can move relative to the parent Camera. Supports move to an absolute location or relative
to current location. Both move functions move the children as well. */
struct Camera : public Obj {
    ImagePlane m_imgPlane;
    FVector3 m_direction;
    std::vector<Obj> m_children;

    Camera() : m_imgPlane{ ImagePlane() }, m_direction{ 0, 0, 1 } {};
    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0, 0, 0 } );

    // Move camera to given absolute position
    void move( const FVector3& abs_pos );
    // Move camera to given relative position
    void moveRel( const FVector3& rel_pos );

    // Generate Camera Ray for Image Plane pixel (x, y)
    FVector3 generateRay( const int x, const int y ) const;

    // Returns the color of the Triangle closest to the camera ray
    Color getTriangleIntersection(
        const FVector3 ray,
        const std::vector<Triangle>& triangles,
        const Camera& camera
    ) const;
};

#endif // CAMERA_H