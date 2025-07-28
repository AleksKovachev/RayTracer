#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "Bases.h"
#include "Triangle.h"

// Camera Image Plane object with a given resolution and position.
struct ImagePlane {
    FVector2 resolution;
    float distanceFromCamera;

    ImagePlane( int width = 1920, int height = 1080, float distFromCamera = 1.f )
        : resolution{ static_cast<float>(width), static_cast<float>(height) },
        distanceFromCamera{ distFromCamera } {
    }
};

/* Camera class with an Image Plane, position, orientation and children. Any child object
can move relative to the parent Camera. Supports move to an absolute location or relative
to current location. Both Move functions move the children as well. */
struct Camera : public Obj {
    ImagePlane m_imgPlane;
    std::vector<Obj*> m_children;
    float m_distToPlane;
    FVector2 m_aspectRatio;

    Camera() : m_imgPlane{ ImagePlane() } { init(); };
    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0.f, 0.f, 0.f } ) {
        init();
    };

    virtual FVector3 GetLocation() const override {
        return m_position;
    };

    // Move camera to given relative position
    virtual void Move( const FVector3& relPos ) override;

    // Initializes all variables that need calculation
    void init();

    // Move camera to given absolute position
    void MoveAbs( const FVector3& absPos );

    // Generate a normalized Camera Ray for Image Plane pixel (x, y) with applied orientation
    FVector3 GenerateRay( const int x, const int y ) const;

    // Move Camera forward-backward
    void Dolly( float val );

    // Move Camera left-right (Track)
    void Truck( float val );

    // Move Camera up-down (Crane)
    void Pedestal( float val );

    // Pan camera left-right
    void Pan( const float deg );

    // Tilt camera up-down
    void Tilt( const float deg );

    // Roll camera left-right
    void Roll( const float deg );

    // Rotate Camera around a point at a given distance with given degree
    void RotateAroundPoint( const FVector3&, const FVector3& );

    // Returns the color of the Triangle closest to the camera ray
    Color GetTriangleIntersection(
        const FVector3 ray,
        const std::vector<Triangle>& triangles,
        const Camera& camera
    ) const;

private:

};

#endif // CAMERA_H