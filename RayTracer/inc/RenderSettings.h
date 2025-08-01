#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include <string> // string

#include "Antialiasing.h" // Antialiasing
#include "Colors.h" // Color
#include "RenderMode.h" // RenderMode
#include "Triangle.h" // Triangle
#include "utils.h" // areCharsInString
#include "Vectors.h" // FVector2, FVector3

struct Color;
class FVector3;
struct Material;
class Scene;

constexpr int InvalidIdx = -1;


// Global scene settings.
struct Settings {
	unsigned renderWidth{};
	unsigned renderHeight{};
	unsigned colorDepth{ 8u };
	Color BGColor{};
	bool overrideBGColor{ false };
	std::string saveDir{ "renders" };
	// Define a small epsilon to avoid self - intersection artifacts. Often
	// 1e-3 - 1e-5 is used for ray origins. Smaller value if scene scale is
	// tiny. A good approach is to have an adaptive shadow bias, based on the
	// size of the Triangle hit or the size of the object. Another common
	// technique is to use dot product of the ray direction and the surface
	// Normal. If the light ray is nearly parallel - the bias shuold be higher.
	float shadowBias{ 0.1f };
	// Correction value to compensate floating point errors in refraction calculations.
	float refractBias{ 0.001f };
	// The maximum number of ray bounces. If exceeded - ray returns BG color.
	int pathDepth{ 5 };
	RenderMode renderMode{ RenderMode::Material };
	// Whether the camera rays should hit or ignore the triangle's backfaces.
	bool ignoreBackface{ true };
	unsigned bucketSize{ 24u }; // Bucket size for bucket rendering. Defaults to 24.
	unsigned accTreeMaxDepth{ 15u }; // Maximum depth for building acceleration tree.
	// Maximum number of triangles to store in an AccTree node.
	unsigned maxAABBTriangleCount{ 10u };
	bool outputSRGB{ false }; // Whether the image is saved in linear or sRGB space.
	Antialiasing antialiasing{ Antialiasing::NO }; // Antialiasing type.
	// The number of times each pixel gets sub-divided when calculating Ray Traced AA.
	unsigned subPixDepthAA{ 0 };
	// The edge detection algorithm to use for FXAA.
	EdgeDetection edgeDetectionTypeFXAA{ EdgeDetection::LUMA };

	// Override the name of the saved file.
	// @param[in] newName: The new name for the rendered image.
	void SetSaveFileName( const std::string& newName ) {
		if ( areCharsInString( newName ) )
			std::exit( 1 );

		saveName = newName;
	}

	// Get the name the file will be saved with.
	// @return The current save file name.
	std::string GetSaveFileName() const {
		return saveName;
	}
private:
	std::string saveName{};
};


// Data crucial for rendering.
struct IntersectionData {
	bool filled{ false };
	FVector3 hitPoint; // The hit point in 3D world space.
	FVector3 faceNormal; // The normal vector of the face at the hit point.
	const Material* material = nullptr; // The material of the hit object.
	// Closes triangle of the closes object to the ray origin.
	int triangleIdx = InvalidIdx;

	Triangle triangle; // The hit triangle. Used until rework is done.
};

#endif // RENDER_SETTINGS_H