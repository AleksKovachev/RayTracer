#ifndef RENDER_H
#define RENDER_H

#include <fstream> // ofstream, ios::binary
#include <limits> // <float>::max
#include <mutex> // mutex, lock_guard
#include <queue> // queue
#include <string> // string, to_string

#include "RenderSettings.h" // RenderMode, IntersectionData, Settings
#include "Vectors.h" // FVector2, FVector3


struct AABBox;
struct Camera;
class ImageBuffer;
struct Ray;
class Scene;
class Triangle;


// A simple struct to hold a bucket information.
struct Bucket {
    unsigned startX, startY, endX, endY;
};


class Render {
public:
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

    // Renders a single image with a single thread.
    void RenderImage();

    // Renders a single image with multiple threads, taking buckets from a pool.
    void RenderBuckets();

    // Used for bucket rendering, but uses acceleration tree traversal.
    // @param[in] region: A structure holding the start and end positions for the region to render.
    // @param[in] buff: The buffer where the rendered colors will be stored.
    void RenderTree( const Bucket&, ImageBuffer& );

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
    const Scene& m_scene; // Reference to the scene object and properties.
    Camera* m_overrideCamera; // Pointer to a camera to use instead of the scene camera.
    const std::string* m_overrideSaveName; // Pointer to a name to override the save file.
    int m_frames; // Number of frames to render for the animation renders.

    // Renders a single bucket of an image. Used with bucket rendering.
    // @param[in] bucketMutex: A mutex used to lock the buckets.
    // @param[in] buckets: A queue of nuckets to render.
    // @param[in] buff: The image buffer to store the data.
    void RenderBucketWorker( std::mutex&, std::queue<Bucket>&, ImageBuffer& );

    // Checks if the current ray intersects the scene's axis aligned bounding box.
    // @param[in] ray: The ray to be checked.
    // @param[in] aabb: The AABB to check if the ray has intersection with.
    // @return If the ray intersects with the AABB.
    bool HasAABBCollision( const Ray&, const AABBox& ) const;

    // Calculates the Barycentric coordinates of a triangle.
    // @param[in] intersectionPt: The intersection point on the triangle to calculate.
    // @param[in] triangle: The triangle for which the coordinates will be calculated.
    // @return An FVector2 with the Barycentric's U and V coordinates. W = 1 - U - V.
    static FVector2 CalcBaryCoords( const FVector3&, const Triangle& );

    // Gets the color of the hit point as a barycentric coordinates debug color.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeBary( const IntersectionData& ) const;

    // Outputs pixel colors based on the normal vector hit.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeNormals( const IntersectionData& ) const;

    // Calculates an interpolated normal for a triangle based on its Barycentric coordinates.
    // @param[in] intersectionPt: The intersection point on the triangle to calculate.
    // @param[in] triangle: The triangle for which the coordinates will be calculated.
    // @return An FVector3 containing the normalized interpolated normal vector.
    static FVector3 CalcHitNormal( const FVector3&, const Triangle& );

    // Calculates the procedural Edges texture color for a given point.
    // @param[in] data: The intersection data.
    // @return The color based on the calculations.
    static Color GetEdgesColor( const IntersectionData& );

    // Calculates the procedural Checker texture color for a given point.
    // @param[in] data: The intersection data.
    // @return The color based on the calculations.
    static Color GetCheckerColor( const IntersectionData& );

    // Calculates the bitmap texture color for a given point.
    // @param[in] data: The intersection data.
    // @return The color based on the calculations.
    static Color GetBitmapColor( const IntersectionData& );

    // Checks the texture type of the current'triangle's material and calculates a color.
    // @param[in] data: The intersection data.
    // @return The calculated/loaded color.
    Color GetRenderColor( const IntersectionData& ) const;

    // Gets the color of a non-reflective, opaque mesh and calculates shadows.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeDiffuse( const IntersectionData& ) const;

    // Gets the color of a reflective mesh and calculates multiple ray bounces.
    // @param[in] ray: The ray to trace, containing direction and depth.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeReflective( const Ray&, const IntersectionData& ) const;

    // Calculate Fresnel reflection factor using Schlick's Approximation.
    // @param[in] ior1: The index of refraction of the current environment.
    // @param[in] ior2: The index of refraction of the environment the ray hit.
    // @return The fresnel scalar multiplier.
    static float CalcFresnelSchlickApprox( const float, const float, const float );

    // Gets the color of a refractive mesh and calculates multiple ray bounces.
    // @param[in] ray: The ray to trace, containing direction and depth.
    // @param[in] data: The intersection data needed for getting the color.
    // @return The color to render.
    Color ShadeRefractive( const Ray&, const IntersectionData& ) const;

    // Returns the color of the Triangle closest to the camera ray.
    // @param[in-out] ray: The ray to be traced.
    // @param[in] data: All intersection data needed for further calculations.
    // @return A color to render for the current pixel after all calculations.
    Color Shade( const Ray&, const IntersectionData& ) const;

    // Traces a ray and checks if it hits any mesh triangle, using acceleration trees.
    // @param[in] ray: The ray to trace.
    // @param[in] maxT: A value beyon which calculations should be cut. Used for shadow rays.
    // @return Intersection data used for further color calculations.
    IntersectionData IntersectRay(
        const Ray& ray, const float maxT = std::numeric_limits<float>::max() ) const;

    // Prepares a .PPM file to be filled in with color data.
    // @return A stream to fill in the color data to.
	std::ofstream PrepareScene();
};

#endif // RENDER_H