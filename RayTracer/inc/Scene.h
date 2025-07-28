#ifndef SCENE_H
#define SCENE_H

#include <sstream> // istringstream
#include <string> // string, getline, npos, stoi
#include <vector> // vector

#include "rapidjson/document.h" // Document, Value, Value::ConstArray

#include "Camera.h" // Camera
#include "Colors.h" // Color, ColorMode, Colors::Black
#include "Lights.h" // Light, PointLight
#include "Materials.h" // Material, MaterialType, Texture, TextureType
#include "Mesh.h" // Mesh, PreparedMesh
#include "RenderSettings.h" // Settings, RenderMode


class Scene {
public:
	// @param[in] sceneFilePath: Path to the scene file.
	Scene( const std::string& );

	~Scene();

	// Parse the scene file to get all data
	void ParseSceneFile();

	// Override the name of the saved file
	// @param[in] saveName: The new name for the rendered image.
	void SetSaveFileName( const std::string& );

	// Set a render color mode
	// @param[in] colorMode: The color mode to use for the rendered image.
	void SetColorMode( const ColorMode );

	// Set render resolutions
	// @param[in] width: The width of the rendered image in pixels.
	// @param[in] height: The height of the rendered image in pixels.
	void SetRenderResolution( const int, const int );

	// Get the render mode from the scene settings.
	// @return a render mode enum value.
	const RenderMode& GetRenderMode() const;

	// Sets the render mode for the scene.
	// @param[in] renderMode: The render mode to use for the final image.
	void SetRenderMode( const RenderMode& );

	// Gets all the prepared meshes in the scene.
	// @return A collection of prepared meshes, ready to iterate.
	const std::vector<PreparedMesh>& GetPreparedMeshes() const;

	// Gets all meshes loaded from the scene file.
	// @return A collection of mesh objects.
	const std::vector<Mesh>& GetMeshes() const;

	// Get the scene camera.
	// @return A camera object.
	const Camera& GetCamera() const;

	// Get the scene settings.
	// @return A settings object.
	const Settings& GetSettings() const;

	// Get the scene lights, no matter the type.
	// @return A collection of all light objects in the scene.
	const std::vector<Light*>& GetLights() const;

	// A getter function for the axis aligned bounding box min.
	// @return The value as an FVector3 object.
	const FVector3& GetAABBmin() const;

	// A getter function for the axis aligned bounding box max.
	// @return The value as an FVector3 object.
	const FVector3& GetAABBmax() const;

	// Parse an obj file and load all relevant data
	void ParseObjFile();
private:
	std::string m_filePath;
	std::vector<Mesh> m_meshes; // Scene objects
	std::vector<PreparedMesh> m_rdyMeshes;
	Camera m_camera; // Main scene Camera
	Settings m_settings; // Global scene settings
	std::vector<Light*> m_lights;
	std::vector<Texture> m_textures; // Texture objects
	FVector3 m_AABBmin;
	FVector3 m_AABBmax;

// crtscene file parsing (json)
private:
	// Internal function for parsing the settings tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseSettingsTag( const rapidjson::Document& );

	// Internal function for parsing the camera tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseCameraTag( const rapidjson::Document& );

	// Internal function for parsing the objects tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseObjectsTag( const rapidjson::Document& );

	// Internal function for parsing the lights tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseLightsTag( const rapidjson::Document& );

	// Internal function for parsing the textures tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseTexturesTag( const rapidjson::Document& );

	// Internal function for parsing the materials tag of a crtscene file.
	// @param[in] doc: A rapidjson document object with the parsed json file.
	void ParseMaterialsTag( const rapidjson::Document& );

	// Internal function for loading vertices from an array.
	// @param[in] arr: The array to traverse.
	// @return A collection of FVector3 objects representing the vertices.
	std::vector<FVector3> LoadVertices( const rapidjson::Value::ConstArray& );

// Obj file parsing
private:
	// Parses a single vertex line of an obj file.
	// @param[in-out] iss: The input stream containing the information.
	// @param[in] line: The line that's being parsed.
	// @return A vector object created from the read vertices.
	FVector3 ParseVertexLine( std::istringstream&, const std::string& );

	// Parses a single polygon line of an obj file.
	// @param[in-out] iss: The input stream containing the information.
	// @param[in] line: The line that's being parsed.
	// @param[out] tris: A collection where the parsed vertex indices are stored.
	void ParsePolygonLine( std::istringstream&, const std::string&, std::vector<int>& );

	// Checks if a new object should be created.
	// @param[in] tag: The current tag that's being checked.
	// @param[out] objCount: An object index indicating which mesh is being processed.
	// @param[out] meshGroup: A collection of triangle collections
	// representing meshes and their triangles.
	void CheckNewObject( const std::string&, size_t, std::vector<std::vector<int>>& );
};

#endif // SCENE_H