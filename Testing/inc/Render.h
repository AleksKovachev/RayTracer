#ifndef RENDER_H
#define RENDER_H

#include <fstream> // ofstream, std::ios::binary
#include <string> // string

#include "RenderSettings.h" // IntersectionData

struct Camera;
class Mesh;
struct Ray;
class Scene;
class Triangle;


class Render {
public:
	const Scene& m_scene;
	Camera* m_overrideCamera;
	const std::string* m_overrideSaveName;
	int m_frames; // Number of frames to render for the animation renders.

    // @param[in] scene: The scene object to render.
	Render( const Scene& );
    // @param[in] scene: The scene object to render.
    // @param[in-out] overrideCamera: A camera object to use instead of the
    // one in the scene file.
	Render( const Scene&, Camera& );
    // @param[in] scene: The scene object to render.
    // @param[in-out] overrideCamera: A camera object to use instead of the
    // one in the scene file.
    // @param[in] overrideSaveName: A custom name for the rendered image.
	Render( const Scene&, Camera&, const std::string& );
	~Render();

    // Traces ShadowRay from hit point to light sources.
    // @param[in] ray: The shadow ray to trace.
    // @param[in] data: The intersection data to work with.
    // @param[in] triN: The normal vector of the hit triangle.
    // @param[in] distToLight: The distance to the light source.
    // @return Boolean indicating if the given point is in shadow.
    static bool IsInShadow(
        const FVector3&,
        const IntersectionData&,
        const FVector3&,
        const float
    );

    // Generate a shadow ray and calculate shadowing for a given point.
    // @param[in] intersectionData: Intersection data needed for the calculations.
    // @param[in] triangle: The triangle that got hit by the camera ray.
    // @param[in] hitNormal: An interpolated normal for smooth shading.
    // @return A color for the shading part of the calculation.
    static Color Shade(
        const IntersectionData& intersectionData,
        const Triangle& triangle,
        const FVector3* hitNormal = nullptr );

    // Returns the color of the Triangle closest to the camera ray.
    // @param[in-out] ray: The ray to be traced.
    // @param[in] scene: The scene where the tracing happens, used for settings.
    // @param[in] reflectionDepth: The maximum recursion depth for reflection.
    // @return A color to render for the current pixel after all calculations.
    Color GetTriangleIntersection(
        Ray&,
        const Scene&,
        const unsigned
    ) const;

    // Renders a single image.
    void RenderImage();
    // Renders a camera movement animation around the scene.
    // @param[in] initialPos: The initial camera position.
    // @param[in] moveWith: A vector representing a relative x, y, z
    // offset for the end camera position.
    void RenderCameraMoveAnimation( const FVector3&, const FVector3& );
    // Renders a camera orbiting animation around an object (or a point in space).
    // @param[in] initialPos: The initial camera position.
    // @param[in] rotation: A vector indicating an X, Y, Z angle at which to rotate the camera.
    void RenderRotationAroundObject( const FVector3&, const FVector3& );

private:
    // Prepares a .PPM file to be filled in with color data.
    // @return A stream to fill in the color data to.
	std::ofstream PrepareScene();
};

#endif // RENDER_H