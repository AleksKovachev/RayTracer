#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "rapidjson/document.h"

#include "Bases.h"   // Color
#include "Camera.h"
#include "RenderSettings.h"
#include "Vectors.h" // FVector3


// TODO: Inherit Obj and Movement/Rotation capabilities
// Collection of triangles representing a 3D object
class Mesh {
public:
	Color color;

	Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles )
		: vertices{ verts }, triangles{ triangles }, color{ 255, 0, 0 } {
	}

	std::vector<FVector3> GetVertices() const { return vertices; }
	std::vector<int> GetTriangles() const { return triangles; }
private:
	std::vector<FVector3> vertices; // 3D coordinates for the whole mesh
	std::vector<int> triangles; // Each triple of vertices form a triangle
};


class Scene {
public:
	Scene( const std::string& sceneFileName );

	void ParseSceneFile();

	std::string GetFileName() const { return m_fileName; }
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Camera& GetCamera() const { return m_camera; }
	const Settings& GetSettings() const { return m_settings; }

	void SetSaveFileName( const std::string& saveName );
	void SetColorMode( const ColorMode colorMode );

private:
	std::string m_fileName;
	std::vector<Mesh> m_meshes; // Scene objects
	Camera m_camera; // Main scene Camera
	Settings m_settings; // Global scene settings

private:
	void ParseSettingsTag( const rapidjson::Document& );
	void ParseCameraTag( const rapidjson::Document& );
	void ParseObjectsTag( const rapidjson::Document& );
};

#endif // SCENE_H