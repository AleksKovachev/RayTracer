#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "rapidjson/document.h"

#include "Camera.h"
#include "Lights.h" // Light
#include "Materials.h" // Material
#include "Mesh.h" // Mesh
#include "RenderSettings.h" // Settings, RenderMode


class Scene {
public:
	RenderMode renderMode;

	Scene( const std::string& sceneFileName );

	~Scene();

	void ParseSceneFile();

	std::string GetFileName() const { return m_fileName; }
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Camera& GetCamera() const { return m_camera; }
	const Settings& GetSettings() const { return m_settings; }
	const std::vector<Light*>& GetLights() const { return m_lights; }
	const std::vector<Material>& GetMaterials() const { return m_materials; }

	void SetSaveFileName( const std::string& saveName );
	void SetColorMode( const ColorMode colorMode );
	void SetRenderResolution( const int width, const int height );

private:
	std::string m_fileName;
	std::vector<Mesh> m_meshes; // Scene objects
	Camera m_camera; // Main scene Camera
	Settings m_settings; // Global scene settings
	std::vector<Light*> m_lights;
	std::vector<Material> m_materials;

private:
	void ParseSettingsTag( const rapidjson::Document& );
	void ParseCameraTag( const rapidjson::Document& );
	void ParseObjectsTag( const rapidjson::Document& );
	void ParseLightsTag( const rapidjson::Document& );
	void ParseMaterialsTag( const rapidjson::Document& );
};


#endif // SCENE_H