#include "Bases.h" // Matrix3
#include "Lights.h" // Light
#include "Scene.h" // Scene, Mesh
#include "utils.h" // getRandomColor, areCharsInString
#include "Vectors.h" // FVector3

#include "rapidjson/istreamwrapper.h" // IStreamWrapper

#include <algorithm> // find
#include <cassert> // assert
#include <fstream> // ifstream
#include <filesystem> // path
#include <iostream> // cerr
#include <sstream> // istringstream


template <typename T>
T loadVector3( const rapidjson::Value::ConstArray& arr );
Matrix3 loadMatrix3( const rapidjson::Value::ConstArray& arr );
std::vector<FVector3> loadMeshVerts( const rapidjson::Value::ConstArray& arr );
std::vector<int> loadMeshTris( const rapidjson::Value::ConstArray& arr );


Scene::Scene( const std::string& sceneFilePath )
	: m_filePath{ sceneFilePath } {
	// Create a path of the save file name string
	std::filesystem::path path( m_filePath );
	// Get a string representation of absolute path to save file name
	m_settings.saveName = path.stem().string();
}

Scene::~Scene() {
	for ( Light* light : m_lights )
		delete light;
}

void Scene::ParseSceneFile() {
	std::ifstream ifs( m_filePath );
	assert( ifs.is_open() );

	rapidjson::IStreamWrapper isw( ifs );
	rapidjson::Document doc;
	doc.ParseStream( isw );
	assert( !doc.HasParseError() );

	ParseSettingsTag( doc );
	ParseCameraTag( doc );
	ParseObjectsTag( doc );
	ParseLightsTag( doc );
	ParseMaterialsTag( doc );

	unsigned counter{};
	for ( Mesh& mesh : m_meshes ) {
		Material mat{};
		mat.albedo = getRandomColor();
		mesh.SetMaterialOverride( mat );

		m_rdyMeshes.emplace_back();
		m_rdyMeshes[counter++].PrepMesh( mesh, m_settings.colorMode );
	}
}

void Scene::SetSaveFileName( const std::string& saveName ) {
	if ( areCharsInString( saveName ) )
		std::exit( 1 );

	m_settings.saveName = saveName;
}

void Scene::SetColorMode( const ColorMode colorMode ) {
	m_settings.colorMode = colorMode;
}

void Scene::SetRenderResolution( const int width, const int height ) {
	m_settings.renderWidth = width;
	m_settings.renderHeight = height;
}

const RenderMode& Scene::GetRenderMode() const {
	return m_settings.renderMode;
}

void Scene::SetRenderMode( const RenderMode& renderMode ) {
	m_settings.renderMode = renderMode;
}

int Scene::GetReflectionDepth() const {
	return m_settings.reflectionDepth;
}

const std::vector<PreparedMesh>& Scene::GetPreparedMeshes() const {
	return m_rdyMeshes;
}

const std::vector<Mesh>& Scene::GetMeshes() const {
	return m_meshes;
}

const Camera& Scene::GetCamera() const {
	return m_camera;
}

const Settings& Scene::GetSettings() const {
	return m_settings;
}

const std::vector<Light*>& Scene::GetLights() const {
	return m_lights;
}

void Scene::ParseSettingsTag(const rapidjson::Document& doc) {
	// JSON Tags to look for
	char t_settings[]{ "settings" };
	char t_bgColor[]{ "background_color" };
	char t_imgSettings[]{ "image_settings" };
	char t_width[]{ "width" };
	char t_height[]{ "height" };

	if ( doc.HasMember( t_settings ) && doc[t_settings].IsObject() ) {
		const rapidjson::Value& settings{doc[t_settings]};
		assert( settings.HasMember( t_bgColor ) && settings[t_bgColor].IsArray() );
		m_settings.BGColor = loadVector3<Color>( settings[t_bgColor].GetArray() );

		assert( settings.HasMember( t_imgSettings ) && settings[t_imgSettings].IsObject() );
		const rapidjson::Value& imgSettings{settings[t_imgSettings]};

		assert( imgSettings.HasMember( t_width ) && imgSettings[t_width].IsInt() );
		m_settings.renderWidth = imgSettings[t_width].GetInt();

		assert( imgSettings.HasMember( t_height ) && imgSettings[t_height].IsInt() );
		m_settings.renderHeight = imgSettings[t_height].GetInt();
	}
}

void Scene::ParseCameraTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_camera[]{ "camera" };
	char t_matrix[]{ "matrix" };
	char t_position[]{ "position" };

	if ( doc.HasMember( t_camera ) && doc[t_camera].IsObject() ) {
		const rapidjson::Value& camera{doc[t_camera]};
		assert( camera.HasMember( t_matrix ) && camera[t_matrix].IsArray());
		m_camera.m_orientation = loadMatrix3( camera[t_matrix].GetArray() );

		assert( camera.HasMember( t_position ) && camera[t_position].IsArray());
		m_camera.m_position = loadVector3<FVector3>( camera[t_position].GetArray() );
	}
}

void Scene::ParseObjectsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_objects[]{ "objects" };
	char t_vertices[]{ "vertices" };
	char t_triangles[]{ "triangles" };
	char t_matIdx[]{ "material_index" };

	if ( doc.HasMember( t_objects ) && doc[t_objects].IsArray() ) {
		const rapidjson::Value::ConstArray& objArr = doc[t_objects].GetArray();

		for ( unsigned i{}; i < objArr.Size(); ++i ) {
			assert( objArr[i].IsObject() );
			const rapidjson::Value& mesh{ objArr[i] };
			assert( mesh.HasMember( t_vertices ) && mesh[t_vertices].IsArray() );
			assert( mesh.HasMember( t_triangles ) && mesh[t_triangles].IsArray() );
			m_meshes.emplace_back( loadMeshVerts( mesh[t_vertices].GetArray() ),
				loadMeshTris( mesh[t_triangles].GetArray() ) );

			if ( mesh.HasMember( t_matIdx ) && mesh[t_matIdx].IsInt() )
				m_meshes[i].SetMaterialIdx( mesh[t_matIdx].GetInt() );
		}
	}
}

void Scene::ParseLightsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_lights[]{ "lights" };
	char t_position[]{ "position" };
	char t_intensity[]{ "intensity" };

	if ( doc.HasMember( t_lights ) && doc[t_lights].IsArray() ) {
		const rapidjson::Value::ConstArray& lightsArr = doc[t_lights].GetArray();

		for ( unsigned i{}; i < lightsArr.Size(); ++i ) {
			assert( lightsArr[i].IsObject() );
			const rapidjson::Value& light{ lightsArr[i] };
			assert( light.HasMember(t_position) && light[t_position].IsArray());
			const FVector3 lightPos = loadVector3<FVector3>( light[t_position].GetArray() );

			// Not using assert because if no intensity is specified - simply init it to 1.
			float ligthIntensity = 1.f;
			if ( light.HasMember( t_intensity ) && light[t_intensity].IsNumber() )
				ligthIntensity = static_cast<float>( light[t_intensity].GetDouble() );

			m_lights.emplace_back( new PointLight( lightPos, ligthIntensity ) );
		}
	}
}

void Scene::ParseMaterialsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_materials[]{ "materials" };
	char t_type[]{ "type" };
	char t_albedo[]{ "albedo" };
	char t_smShading[]{ "smooth_shading" };
	char t_ior[]{ "ior" };

	if ( doc.HasMember( t_materials ) && doc[t_materials].IsArray() ) {
		const rapidjson::Value::ConstArray& matsArr = doc[t_materials].GetArray();

		for ( unsigned i{}; i < matsArr.Size(); ++i ) {
			assert( matsArr[i].IsObject() );
			const rapidjson::Value& material{ matsArr[i] };
			assert( material.HasMember( t_type ) && material[t_type].IsString() );
			assert( material.HasMember( t_smShading ) && material[t_smShading].IsBool() );

			Material mat;
			std::string matTypeStr = material[t_type].GetString();

			if ( matTypeStr == "diffuse" )
				mat.type = MaterialType::Diffuse;
			else if ( matTypeStr == "reflective" )
				mat.type = MaterialType::Reflective;
			else if ( matTypeStr == "refractive" ) {
				mat.type = MaterialType::Refractive;
			}
			else if ( matTypeStr == "constant" )
				mat.type = MaterialType::Constant;

			mat.smoothShading = material[t_smShading].GetBool();

			// Assign a value for the IOR if there is one.
			if ( material.HasMember( t_ior ) && material[t_ior].IsDouble() )
				mat.ior = static_cast<float>(material[t_ior].GetDouble());

			// Assign a value for the albedo if there is one.
			if ( material.HasMember( t_albedo ) && material[t_albedo].IsArray())
				mat.albedo = loadVector3<Color>( material[t_albedo].GetArray() );


			for ( Mesh& mesh : m_meshes ) {
				if ( mesh.GetMaterialIdx() == i ) {
					mesh.SetMaterial( mat );
				}
			}
		}
	}
}

template <typename T>
T loadVector3( const rapidjson::Value::ConstArray& arr ) {
	assert( arr.Size() == 3 );
	return { static_cast<float>(arr[0].GetDouble()),
		static_cast<float>(arr[1].GetDouble()),
		static_cast<float>(arr[2].GetDouble()) };
}

Matrix3 loadMatrix3( const rapidjson::Value::ConstArray& arr ) {
	assert( arr.Size() == 9 );
	return { FVector3{ arr[0].GetDouble(), arr[1].GetDouble(), arr[2].GetDouble() },
		FVector3{ arr[3].GetDouble(), arr[4].GetDouble(), arr[5].GetDouble() },
		FVector3{ arr[6].GetDouble(), arr[7].GetDouble(), arr[8].GetDouble() } };
}

std::vector<FVector3> loadMeshVerts( const rapidjson::Value::ConstArray& arr ) {
	std::vector<FVector3> verts{};

	for ( unsigned i{}; i + 2 < arr.Size(); i += 3 )
		verts.emplace_back( static_cast<float>(arr[i].GetDouble()),
			static_cast<float>(arr[i + 1].GetDouble()),
			static_cast<float>(arr[i + 2].GetDouble()));

	return verts;
}

std::vector<int> loadMeshTris( const rapidjson::Value::ConstArray& arr ) {
	std::vector<int> tris{};

	for ( unsigned i{}; i < arr.Size(); ++i )
		tris.emplace_back( arr[i].GetInt() );

	return tris;
}

FVector3 Scene::ParseVertexLine( std::istringstream& iss, const std::string& line ) {
	float val1, val2, val3;

	if ( !(iss >> val1 >> val2 >> val3) )
		std::cerr << "Error parsing vertex line: " << line << "\n";
		// Not returning to avoid warnings or usage of std::optional

	return { val1, val3, val2 }; // Swap val2 and val3 to convert to right-handed
}

void Scene::ParsePolygonLine( std::istringstream& iss, const std::string& line, std::vector<int>& tris ) {
	/* Face tag info:
	* Faces can hold information in one of 4 formats:
	* f 1 2 3 => face with only 'v' indices
	* f 1/1 2/2 3/3 => face with 'v' indices and 'vt' indices
	* f 1//1 2//2 3//3 => face with 'v' and 'vn' indices, but no 'vt' info
	* f 1/1/1 2/2/2 3/3/3 => face with 'v', 'vt', and 'vn' indices
	*/

	std::string vertexData;
	std::vector<int> vertexIndices{};

	while ( iss >> vertexData ) {
		// Get vertex index
		size_t pos = vertexData.find( '/' );
		if ( pos == std::string::npos ) {
			// The face has no 'vt' and 'vn' tags, just vertex indices
			vertexIndices.push_back( stoi( vertexData ) );
			continue;
		}
		vertexIndices.push_back( std::stoi( vertexData.substr(0, pos) ) );
		// Ignore 'vt' and 'vn'
	}

	if ( vertexIndices.size() < 3 )
		std::cerr << "Indices in face: " << line << "  are less than 3. Skipping...\n";
	else if ( vertexIndices.size() == 3 )
		tris.insert( tris.end(), { vertexIndices[0], vertexIndices[2], vertexIndices[1] } );
	else if ( vertexIndices.size() > 4 )
		std::cerr << "Indices in face: " << line << "  are more than 4. Skipping...\n";
	else {
		// There are 4 indices - it's a quad face - using fan triangulation
		tris.insert( tris.end(),
			{ vertexIndices[0], vertexIndices[1], vertexIndices[2],
			vertexIndices[0], vertexIndices[2], vertexIndices[3] } );
	}
}

void Scene::CheckNewObject(
	const std::string& tag,
	size_t objCount,
	std::vector<std::vector<int>>& meshGroup
) {
	static std::vector<std::string> parsedTags{};
	static std::string lastTag{ "<INITIAL>" };

	// If the tag is the same as the last -> it's iterating over objects
	if ( tag == lastTag )
		return;

	lastTag = tag;

	auto it = std::find( parsedTags.begin(), parsedTags.end(), tag );

	// If tag was already scanned => create an object and prepare for next one
	if ( it != parsedTags.end() ) {
		objCount++;
		meshGroup.push_back( std::vector<int>() );
		parsedTags.clear();
	}
	parsedTags.push_back( tag );
}

void Scene::ParseObjFile() {
	/* OBJ file format Rules
	* 1. Tags don't have particular order.
	*    Any tag can be placed before or after any other.
	* 2. There's nothing to mark the start or end of a new mesh
	* 3. Vertex and Traingle tags are in a global context. Any polygon ("f" tag)
	*    can reference any vertex in the file, no matter if it's placed under
	*    the same or different object - before or after the current "f" tag.
	* 4. Vertex normals are not stored per vertex, but separatelly as they are
	*    reused in faces (like vertices).
	* 5. Vertex normals are not stored in the vertices themselves (as I do here)
	*    but rather in every face. Different faces may use different vertex
	*    normal depending if the edge next to that face is sharp or smooth.
	*    This is closely relates to the "s" tag for the smoothing groups.
	* 6. The Polygons "f" tag store information in N groups of 3 elements. The N
	*    corresponds to the number of vertices the face is constructed from
	*    (usually 3 or 4, but could be any N-gon). The 1st element is the vertex
	*    index "v", the 2nd - vertex texture coordinate "vt" index, and the 3rd
	*    is the vertex normal "vn" index.
	* 
	*   In this parser:
	*   Ignore vertex normals "vn"
	*   Ignore vertex texture coordinates "vt"
	*   Ignore Object tag "o"
	*   Ignore groups as they're not supported here "g"
	*   Ignore "usemtl" as no information in mtl files is used here
	*   Ignore "mtllib" for same reason as above
	*   Ignore smoothing groups as they're not supported here "s"
	*   Ignore all other, more uncommon tags
	*/

	// Prepare scene settings missing in obj file
	m_settings.renderWidth = 1920;
	m_settings.renderHeight = 1080;
	m_settings.BGColor = Colors::Black;
	m_camera.Dolly( 3 );
	//m_camera.RotateAroundPoint( { 0.f, 0.f, 6.f }, { 90.f, 0.f, 0.f } );
	//PointLight* ptLight = new PointLight{ {2.f, 3.f, 2.f}, 170 };
	PointLight* ptLight = new PointLight{ {0.f, 5.f, 0.f}, 8000 };
	PointLight* ptLight2 = new PointLight{ {0.f, -5.f, 0.f}, 8000 };
	PointLight* ptLight3 = new PointLight{ {0.f, 0.f, 5.f}, 4000 };
	PointLight* ptLight4 = new PointLight{ {0.f, 0.f, -5.f}, 4000 };
	PointLight* ptLight5 = new PointLight{ {5.f, 0.f, 0.f}, 4000 };
	PointLight* ptLight6 = new PointLight{ {-5.f, 0.f, 0.f}, 4000 };
	//PointLight* ptLight2 = new PointLight{ {-4.f, 2.f, -1.f}, 170 };
	//PointLight* ptLight3 = new PointLight{ {4.f, -2.f, -1.f}, 170 };
	m_lights.push_back( ptLight );
	m_lights.push_back( ptLight2 );
	m_lights.push_back( ptLight3 );
	m_lights.push_back( ptLight4 );
	m_lights.push_back( ptLight5 );
	m_lights.push_back( ptLight6 );

	std::ifstream file( m_filePath );
	assert( file.is_open() );

	std::string line;
	std::string tag;
	std::vector<int> triangles;
	std::vector<FVector3> vertices;
	//std::vector<FVector3> currTriangle;
	std::vector<std::vector<int>> meshGroup( 1 );
	size_t objCount{};

	while ( std::getline( file, line ) ) {
		if ( line.empty() )
			continue;

		std::istringstream iss( line );
		if ( !(iss >> tag) ) {
			std::cerr << "Error reading object type: " << tag << ". Skipping...\n";
			continue;
		}

		// skip comments
		if ( tag[0] == '#' )
			continue;

		// Parse vertices
		if ( tag == "v" ) {
			vertices.push_back( ParseVertexLine( iss, line ) );
			CheckNewObject( "v", objCount, meshGroup );
			continue;
		}

		// Parse polygons
		if ( tag == "f" ) {
			// Check number of vertices in face
			// If it's 3 - add the triangle
			// If it's 4 - use fan triangulation - 123, 134 for convex polygons
			// If it's > 4 -> don't support as there's no time for
			// implementation of ear-clipping algorithm :(
			CheckNewObject( "f", objCount, meshGroup );
			ParsePolygonLine( iss, line, triangles );
			meshGroup[objCount].insert(
				meshGroup[objCount].end(),
				triangles.begin(),
				triangles.end()
			);
			triangles.clear();
			continue;
		}
	}

	//! Rework!< Not working at the moment!!!
	for ( std::vector<int>& prepMesh : meshGroup ) {
		Mesh mesh( vertices, prepMesh );
		Material mat;
		mat.type = MaterialType::Diffuse;
		mat.albedo = getRandomColor();
		mat.smoothShading = false;
		mesh.SetMaterial( mat );
		m_meshes.push_back( mesh );
	}
}
