#ifndef CAMERA_H
#define CAMERA_H

#include <vector> // vector

#include "Bases.h" // Obj
#include "Rays.h" // Ray, RayType
#include "Vectors.h" // FVector2, Vector3

class Scene;
struct Settings;

// Camera Image Plane object with a given resolution and position.
struct ImagePlane {
    FVector2 resolution; // Width and height in pixels.
    float distanceFromCamera;
    FVector2 m_aspectRatio; // The image plane aspect ratio as a rational number.

    // @param[in] width: The width of the image plane in pixels. Defaults to 1920.
    // @param[in] height: The height of the image plane in pixels. Defaults to 1080.
    // @param[in] height: The distance of the image plane from the camera.
    // Defaults to 1 unit (meter).
    ImagePlane( int width = 1920, int height = 1080, float distFromCamera = 1.f );

    // Initializes all variables that need calculation.
    void init();
};

/* Camera class with an Image Plane, position, orientation and children. Any child object
can move relative to the parent Camera. Supports move to an absolute location or relative
to current location. Both Move functions move the children as well. */
struct Camera : public Obj {
    ImagePlane m_imgPlane;
    std::vector<Obj*> m_children; // All objects attached to this camera.

    Camera();
    // @param[in] imgPlane: An image plane for the camera.
    // @param[in] direction: A direction for the camera.
    // @param[in] pos: A point in world space where the camera is located.
    // Defaults to (x=0.0f, y=0.0f, z=0.0f).
    Camera(
        const ImagePlane& imgPlane,
        const FVector3& direction,
        const FVector3& pos = { 0.f, 0.f, 0.f }
    );

    ~Camera();

    // Returns Camera location.
    // @return The camera location in world space.
    virtual FVector3 GetLocation() const override;

    // Move camera to given relative position.
    // @param[in] relPos: Relative X, Y, Z offset.
    virtual void Move( const FVector3& ) override;

    // Generate a normalized Camera Ray for Image Plane pixel (x, y) with applied orientation.
    // @param[in] x: Resolution width.
    // @param[in] y: Resolution height.
    // @param[in] settings: Scene settings.
    // @return The generated camera ray.
    Ray GenerateRay( const int, const int, const Settings& ) const;

    // Move Camera forward-backward.
    // @param[in] val: The number of units the Camera will move.
    // Negative moves in the opposite direction.
    void Dolly( const float );

    // Move Camera left-right (Track).
    // @param[in] val: The number of units the Camera will move.
    // Negative moves in the opposite direction.
    void Truck( const float );

    // Move Camera up-down (Crane).
    // @param[in] val: The number of units the Camera will move.
    // Negative moves in the opposite direction.
    void Pedestal( const float );

    // Pan camera left-right.
    // @param[in] deg: The angle in degres at which Camera will rotate.
    void Pan( const float );

    // Tilt camera up-down.
    // @param[in] deg: The angle in degres at which Camera will rotate.
    void Tilt( const float );

    // Roll camera left-right.
    // @param[in] deg: The angle in degres at which Camera will rotate.
    void Roll( const float );

    // Rotate Camera around a point at a given distance at a given angle.
    // @param[in] pivot: The point, around which the camera will rotate.
    // @param[in] angle: The angle at which the camera will rotate each frame.
    void RotateAroundPoint( const FVector3&, const FVector3& );
};

#endif // CAMERA_H