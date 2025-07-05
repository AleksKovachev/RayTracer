#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "Bases.h" // Obj
#include "Colors.h" // Color, Colors::Black
#include "Mesh.h" // PreparedMesh
#include "RenderSettings.h" // IntersectionData
#include "Triangle.h"
#include "Vectors.h" // FVector3

class Scene;

// Camera Image Plane object with a given resolution and position.
struct ImagePlane {
    FVector2 resolution;
    float distanceFromCamera;

    ImagePlane( int width = 1920, int height = 1080, float distFromCamera = 1.f );
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
    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0.f, 0.f, 0.f } );

    ~Camera();

    virtual FVector3 GetLocation() const override;

    // Move camera to given relative position
    virtual void Move( const FVector3& relPos ) override;

    // Initializes all variables that need calculation
    void init();

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
    void RotateAroundPoint( const FVector3& dist, const FVector3& angle );

    // Traces ShadowRay from hit point to light sources.
    static bool IsInShadow( const FVector3& ray, const IntersectionData& data );

    // Returns the color of the Triangle closest to the camera ray
    Color GetTriangleIntersection(
        const FVector3& ray,
        const std::vector<PreparedMesh>& meshes,
        const Scene& scene
    ) const;

private:

};

#endif // CAMERA_H