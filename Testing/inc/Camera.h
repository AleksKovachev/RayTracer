#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "Bases.h"
#include "Triangle.h"

// Camera Image Plane object with a given resolution and position.
struct ImagePlane : Obj {
    FVector2 resolution;

    ImagePlane( int width = 1920, int height = 1080, Vector3 pos = { 0, 0, -1 } )
        : resolution{ static_cast<float>(width), static_cast<float>(height) }, Obj( pos ) {
    }

    virtual FVector3 getLocation() const override {
        return m_position;
    }

    virtual void move( const FVector3& vec ) override {
        m_position = vec;
    }

    virtual void rotate( const FVector3& vec ) override {
        m_rotation = vec;
    }
};

/* Camera class with an Image Plane, position, orientation and children. Any child object
can move relative to the parent Camera. Supports move to an absolute location or relative
to current location. Both move functions move the children as well. */
struct Camera : public Obj {
    ImagePlane m_imgPlane;
    FVector3 m_direction;
    std::vector<Obj*> m_children;
    float m_distToPlane;
    FVector2 m_aspectRatio;

    Camera() : m_imgPlane{ ImagePlane() }, m_direction{ 0.f, 0.f, 1.f } { init(); };
    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0.f, 0.f, 0.f } ) {
        init();
    };

    virtual FVector3 getLocation() const override {
        return m_position;
    };

    // Move camera to given absolute position
    virtual void move( const FVector3& pos ) override;

    // Initializes all variables that need calculation
    void init();

    // Move camera to given relative position
    void moveRel( const FVector3& rel_pos );

    // Generate a normalized Camera Ray for Image Plane pixel (x, y) with applied orientation
    FVector3 generateRay( const int x, const int y ) const;

    // Move Camera forward-backward
    void dolly( float val );

    // Move Camera left-right (Track)
    void truck( float val );

    // Move Camera up-down (Crane)
    void pedestal( float val );

    // Returns the color of the Triangle closest to the camera ray
    Color getTriangleIntersection(
        const FVector3 ray,
        const std::vector<Triangle>& triangles,
        const Camera& camera
    ) const;

private:

};

#endif // CAMERA_H