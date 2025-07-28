#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "Bases.h"

// Camera Image Plane object with a given resolution and position.
struct ImagePlane : Obj {
    FVector2 resolution;
    ImagePlane( int width, int height, Vector3 pos = { 0, 0, -1 } )
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

    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0, 0, 0 } );

    void move( const FVector3& abs_pos );
    void moveRel( const FVector3& rel_pos );
};

#endif // CAMERA_H