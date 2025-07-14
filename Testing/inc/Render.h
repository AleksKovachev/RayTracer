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
    // @param[in] distToLight: The distance to the light source.
    // @return Boolean indicating if the given point is in shadow.
    bool IsInShadow( const Ray&, const float ) const;

    // Returns the color of the Triangle closest to the camera ray.
    // @param[in-out] ray: The ray to be traced.
    // @param[in] data: All intersection data needed for further calculations.
    // @return A color to render for the current pixel after all calculations.
    Color Shade( const Ray& ray, const IntersectionData& data ) const;

    // Gets the color of the hit mesh or triangle, depending on color mode.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeConstant( const IntersectionData& data ) const;
    // Gets the color of the hit point as a barycentric coordinates debug color.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeBary( const IntersectionData& data ) const;
    // Gets the color of a non-reflective, opaque mesh and calculates shadows.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeDiffuse( const IntersectionData& data ) const;
    // Gets the color of a reflective mesh and calculates multiple ray bounces.
    // @param[in] ray: The ray to trace, containing direction and depth.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeReflective( const Ray& ray, const IntersectionData& data ) const;
    // Calculate Fresnel reflection factor using Schlick's Approximation.
    // @param[in] ior1: The index of refraction of the current environment.
    // @param[in] ior2: The index of refraction of the environment the ray hit.
    // @return The fresnel scalar multiplier.
    static float CalcFresnelSchlickApprox( const float, const float, const float );
    // Gets the color of a refractive mesh and calculates multiple ray bounces.
    // @param[in] ray: The ray to trace, containing direction and depth.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeRefractive( const Ray& ray, const IntersectionData& data ) const;
    // Traces a ray and checks if it hits any mesh triangle.
    // @param[in] ray: The ray to trace.
    // @param[in] maxT: A value beyon which calculations should be cut. Used for shadow rays.
    // @return Intersection data used for further color calculations.
    IntersectionData TraceRay(
        const Ray& ray, const float maxT = std::numeric_limits<float>::max() ) const;


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