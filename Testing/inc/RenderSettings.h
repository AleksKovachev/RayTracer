#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include <string> // string

#include "Mesh.h" // PreparedMesh
#include "Vectors.h" // FVector2, FVector3

enum class ColorMode;
struct Color;
class Scene;
class FVector3;

constexpr int InvalidIdx = -1;

// Enum for choosing render style
enum class RenderMode {
	// Render the object with its override material. Either mesh or triangle color.
	ObjectColor,
	Barycentric, // Render object visualizing the Barycentric coordinates.
	Normals, // Render object using normal vectors as colors.
	ShadedNormals, // Render object using normal vectors as colors (with shading).
	Material // Render the object with its material.
};

// Global scene settings
struct Settings {
	int renderWidth;
	int renderHeight;
	int maxColorComp; // The maximum color component for the scene (color-depth).
	Color BGColor;
	std::string saveDir;
	std::string saveName;
	ColorMode colorMode;
	// Define a small epsilon to avoid self - intersection artifacts. Often
	// 1e-3 - 1e-5 is used for ray origins. Smaller value if scene scale is
	// tiny. A good approach is to have an adaptive shadow bias, based on the
	// size of the Triangle hit or the size of the object. Another common
	// technique is to use dot product of the ray direction and the surface
	// Normal. If the light ray is nearly parallel - the bias shuold be higher.
	float shadowBias;
	// Correction value to compensate floating point errors in refraction calculations.
	float refractBias;
	// The maximum number of ray bounces. If exceeded - ray returns BG color.
	int pathDepth;
	RenderMode renderMode;
	// Whether the camera rays should hit or ignore the triangle's backfaces.
	bool ignoreBackface;

	// @param[in] colorDepth: The color's bit depth. Defaults to 8-bit colors.
	Settings( const int colorDepth = 8 );
};


// A Structure that holds data crucial for rendering
struct IntersectionData {
	FVector3 hitPoint; // The hit point in 3D world space.
	FVector3 faceNormal; // The normal vector of the face at the hit point.
	// The normal in the hit point interpolated by the vertex normals of the triangle.
	FVector3 interpVertNormal;
	FVector2 baryCoords; // Where the triangle is hit.
	const Material* material = nullptr; // The material of the hit object.
	int objectIdx = InvalidIdx; // Closes object to the ray origin.
	// Closes triangle of the closes object to the ray origin.
	int triangleIdx = InvalidIdx;

	Triangle triangle; // The hit triangle. Used until rework is done.
};

#endif // RENDER_SETTINGS_H
