#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include <string>

#include "Mesh.h" // PreparedMesh

enum class ColorMode;
struct Color;
class Scene;
class FVector3;

// Enum for choosing render style
enum class RenderMode {
	ObjectColor,
	Barycentric,
	Material
};

// Global scene settings
struct Settings {
	int renderWidth;
	int renderHeight;
	int maxColorComp;
	Color BGColor;
	std::string saveDir;
	std::string saveName;
	ColorMode colorMode;
	float shadowBias;
	int reflectionDepth;
	RenderMode renderMode;

	Settings( const int colorDepth = 8 );
};


// A Structure that holds data crucial for rendering
struct IntersectionData {
	IntersectionData(
		const std::vector<PreparedMesh>& meshes,
		const Material& currMeshMat,
		const Scene& scene,
		const FVector3& intersectionPt
	) : meshes{ meshes },
		currMeshMat{ currMeshMat },
		scene{ scene },
		intersectionPt{ intersectionPt } {
	}

	const std::vector<PreparedMesh>& meshes;
	const Material& currMeshMat;
	const Scene& scene;
	const FVector3& intersectionPt;
};


#endif // RENDER_SETTINGS_H
