#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include "rapidjson/document.h"

#include "Camera.h"
#include "Lights.h"
#include "Mesh.h"
#include "RenderSettings.h"


class Scene {
public:
	Scene( const std::string& sceneFileName );

	~Scene();

	void ParseSceneFile();

	std::string GetFileName() const { return m_fileName; }
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Camera& GetCamera() const { return m_camera; }
	const Settings& GetSettings() const { return m_settings; }
	const std::vector<Light*>& GetLights() const { return m_lights; }

	void SetSaveFileName( const std::string& saveName );
	void SetColorMode( const ColorMode colorMode );

private:
	std::string m_fileName;
	std::vector<Mesh> m_meshes; // Scene objects
	Camera m_camera; // Main scene Camera
	Settings m_settings; // Global scene settings
	std::vector<Light*> m_lights;

private:
	void ParseSettingsTag( const rapidjson::Document& );
	void ParseCameraTag( const rapidjson::Document& );
	void ParseObjectsTag( const rapidjson::Document& );
	void ParseLightsTag( const rapidjson::Document& );
};


#endif // SCENE_H