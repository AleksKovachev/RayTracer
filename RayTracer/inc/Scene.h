#ifndef SCENE_H
#define SCENE_H

#include <string> // string, getline
#include <vector>

#include "rapidjson/document.h"

#include "Camera.h"
#include "Lights.h" // Light
#include "Materials.h" // Material
#include "Mesh.h" // Mesh
#include "RenderSettings.h" // Settings, RenderMode


class Scene {
public:

	Scene( const std::string& sceneFileName );

	~Scene();

	// Parse the scene file to get all data
	void ParseSceneFile();

	// Parse an obj file and load all relevant data
	void ParseObjFile();

	std::string GetFileName() const { return m_fileName; }
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Camera& GetCamera() const { return m_camera; }
	const Settings& GetSettings() const { return m_settings; }
	const std::vector<Light*>& GetLights() const { return m_lights; }

	// Define the name of the saved file
	void SetSaveFileName( const std::string& saveName );

	// Set a render color mode
	void SetColorMode( const ColorMode colorMode );

	// Set render resolutions
	void SetRenderResolution( const int width, const int height );

	const RenderMode& GetRenderMode() const;

	void SetRenderMode( const RenderMode& renderMode );

	unsigned GetReflectionDepth() const;

private:
	std::string m_fileName;
	std::vector<Mesh> m_meshes; // Scene objects
	Camera m_camera; // Main scene Camera
	Settings m_settings; // Global scene settings
	std::vector<Light*> m_lights;

// crtscene file parsing (json)
private:
	void ParseSettingsTag( const rapidjson::Document& );
	void ParseCameraTag( const rapidjson::Document& );
	void ParseObjectsTag( const rapidjson::Document& );
	void ParseLightsTag( const rapidjson::Document& );
	void ParseMaterialsTag( const rapidjson::Document& );

// Obj file parsing
private:
	FVector3 parseVertexLine( std::istringstream& iss, const std::string& line );
	void parsePolygonLine( std::istringstream& iss, const std::string& line, std::vector<int>& tris );
	// Checks if a new object should be created
	void checkNewObject(
		const std::string& tag,
		size_t objCount,
		std::vector<std::vector<int>>& meshGroup );
};


#endif // SCENE_H