#include "Bases.h" // Matrix3
#include "Lights.h" // Light
#include "Scene.h" // Scene
#include "utils.h" // getRandomColor
#include "Vectors.h" // FVector3

#define STB_IMAGE_IMPLEMENTATION // Only needed in a single .cpp file.
#include "stb_image.h" // stbi_load
#include "rapidjson/istreamwrapper.h" // IStreamWrapper

#include <algorithm> // find, min, max
#include <cassert> // assert
#include <fstream> // ifstream
#include <filesystem> // path
#include <iostream> // cerr


template <typename T>
T loadVector3( const rapidjson::Value::ConstArray& arr );
Matrix3 loadMatrix3( const rapidjson::Value::ConstArray& arr );
std::vector<FVector3> loadUVs( const rapidjson::Value::ConstArray& arr );


Scene::Scene() : m_accTree{ *this } {}

Scene::Scene( const std::string& sceneFilePath )
	: m_filePath{ sceneFilePath }, m_aabb{}, m_accTree{ *this } {
	// Create a path of the save file name string
	std::filesystem::path path( m_filePath );
	// Get a string representation of absolute path to save file name
	settings.SetSaveFileName( path.stem().string() );
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
	ParseTexturesTag( doc );
	ParseMaterialsTag( doc );

	// Using SAH eliminates the need for fine tuning maxDepth and maxBoxTriCount.
	// Default formula: static_cast<int>( 8 + 1.3 * log2f( static_cast<float>( m_triangles.size() ) ) );
	// Formula without the (1.3 *) part also exists.
	m_accTree.maxDepth = settings.accTreeMaxDepth;
	// Typical 1-10. 1-4 for high perf. 5-10 for fast tree building. 4-8 is okay for GPU rendering.
	m_accTree.maxBoxTriangleCount = settings.maxAABBTriangleCount;
	int rootIdx = m_accTree.AddNode( m_aabb, *this );
	m_accTree.Build( rootIdx, 0, m_triIndices );
}

const std::vector<Triangle>& Scene::GetTriangles() const {
	return m_triangles;
}

const Camera& Scene::GetCamera() const {
	return m_camera;
}

const std::vector<Light*>& Scene::GetLights() const {
	return m_lights;
}

const AABBox& Scene::GetAABB() const {
	return m_aabb;
}

void Scene::ParseSettingsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_settings[]{ "settings" };
	char t_bgColor[]{ "background_color" };
	char t_imgSettings[]{ "image_settings" };
	char t_width[]{ "width" };
	char t_height[]{ "height" };
	char t_bucketSize[]{ "bucket_size" };

	if ( doc.HasMember( t_settings ) && doc[t_settings].IsObject() ) {
		const rapidjson::Value& settings_{ doc[t_settings] };
		assert( settings_.HasMember( t_bgColor ) && settings_[t_bgColor].IsArray() );
		if ( !settings.overrideBGColor )
			settings.BGColor = loadVector3<Color>( settings_[t_bgColor].GetArray() );

		assert( settings_.HasMember( t_imgSettings ) && settings_[t_imgSettings].IsObject() );
		const rapidjson::Value& imgSettings{ settings_[t_imgSettings] };

		assert( imgSettings.HasMember( t_width ) && imgSettings[t_width].IsInt() );
		settings.renderWidth = imgSettings[t_width].GetUint();

		assert( imgSettings.HasMember( t_height ) && imgSettings[t_height].IsInt() );
		settings.renderHeight = imgSettings[t_height].GetUint();

		if ( imgSettings.HasMember( t_bucketSize ) && imgSettings[t_bucketSize].IsInt() ) {
			settings.bucketSize = imgSettings[t_bucketSize].GetUint();
		}
	}
}

void Scene::ParseCameraTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_camera[]{ "camera" };
	char t_matrix[]{ "matrix" };
	char t_position[]{ "position" };

	if ( doc.HasMember( t_camera ) && doc[t_camera].IsObject() ) {
		const rapidjson::Value& camera{ doc[t_camera] };
		assert( camera.HasMember( t_matrix ) && camera[t_matrix].IsArray() );
		m_camera.m_orientation = loadMatrix3( camera[t_matrix].GetArray() );

		assert( camera.HasMember( t_position ) && camera[t_position].IsArray() );
		m_camera.m_position = loadVector3<FVector3>( camera[t_position].GetArray() );
	}
}

void Scene::ParseObjectsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_objects[]{ "objects" };
	char t_vertices[]{ "vertices" };
	char t_triangles[]{ "triangles" };
	char t_matIdx[]{ "material_index" };
	char t_uvs[]{ "uvs" };

	if ( doc.HasMember( t_objects ) && doc[t_objects].IsArray() ) {
		const rapidjson::Value::ConstArray& objArr = doc[t_objects].GetArray();

		for ( unsigned i{}; i < objArr.Size(); ++i ) {
			assert( objArr[i].IsObject() );
			const rapidjson::Value& mesh{ objArr[i] };
			assert( mesh.HasMember( t_vertices ) && mesh[t_vertices].IsArray() );
			assert( mesh.HasMember( t_triangles ) && mesh[t_triangles].IsArray() );

			int matIdx = -1;
			if ( mesh.HasMember( t_matIdx ) && mesh[t_matIdx].IsInt() )
				matIdx = mesh[t_matIdx].GetInt();

			std::vector<FVector3> UVs{};
			if ( mesh.HasMember( t_uvs ) && mesh[t_uvs].IsArray() )
				UVs = loadUVs( mesh[t_uvs].GetArray() );

			std::vector<FVector3> meshVerts = LoadVertices( mesh[t_vertices].GetArray() );
			LoadMesh( mesh[t_triangles].GetArray(), meshVerts, UVs, matIdx );


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
			assert( light.HasMember( t_position ) && light[t_position].IsArray() );
			const FVector3 lightPos = loadVector3<FVector3>( light[t_position].GetArray() );

			// Not using assert because if no intensity is specified - simply init it to 1.
			float ligthIntensity = 1.f;
			if ( light.HasMember( t_intensity ) && light[t_intensity].IsNumber() )
				ligthIntensity = static_cast<float>( light[t_intensity].GetDouble() );

			m_lights.emplace_back( new PointLight( lightPos, ligthIntensity ) );
		}
	}
}

void Scene::ParseTexturesTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_textures[]{ "textures" };
	char t_name[]{ "name" };
	char t_type[]{ "type" };
	char t_albedo[]{ "albedo" };
	char t_edgeColor[]{ "edge_color" };
	char t_innerColor[]{ "inner_color" };
	char t_edgeWidth[]{ "edge_width" };
	char t_colorA[]{ "color_A" };
	char t_colorB[]{ "color_B" };
	char t_squareSize[]{ "square_size" };
	char t_filePath[]{ "file_path" };

	if ( doc.HasMember( t_textures ) && doc[t_textures].IsArray() ) {
		const rapidjson::Value::ConstArray& texArr = doc[t_textures].GetArray();

		for ( unsigned i{}; i < texArr.Size(); ++i ) {
			assert( texArr[i].IsObject() );
			const rapidjson::Value& texture{ texArr[i] };
			assert( texture.HasMember( t_name ) && texture[t_name].IsString() );
			assert( texture.HasMember( t_type ) && texture[t_type].IsString() );

			Texture tex{};
			tex.name = texture[t_name].GetString();

			if ( texture[t_type] == "albedo" ) {
				tex.type = TextureType::SolidColor;
				assert( texture.HasMember( t_albedo ) && texture[t_albedo].IsArray() );
				tex.albedo = loadVector3<Color>( texture[t_albedo].GetArray() );
			} else if ( texture[t_type] == "edges" ) {
				tex.type = TextureType::EdgesP;
				assert( texture.HasMember( t_edgeColor ) && texture[t_edgeColor].IsArray() );
				assert( texture.HasMember( t_innerColor ) && texture[t_innerColor].IsArray() );
				assert( texture.HasMember( t_edgeWidth ) && texture[t_edgeWidth].IsDouble() );
				tex.colorA = loadVector3<Color>( texture[t_edgeColor].GetArray() );
				tex.colorB = loadVector3<Color>( texture[t_innerColor].GetArray() );
				tex.scalar = static_cast<float>(texture[t_edgeWidth].GetDouble());
			} else if ( texture[t_type] == "checker" ) {
				tex.type = TextureType::CheckerP;
				assert( texture.HasMember( t_colorA ) && texture[t_colorA].IsArray() );
				assert( texture.HasMember( t_colorB ) && texture[t_colorB].IsArray() );
				assert( texture.HasMember( t_squareSize ) && texture[t_squareSize].IsDouble() );
				tex.colorA = loadVector3<Color>( texture[t_colorA].GetArray() );
				tex.colorB = loadVector3<Color>( texture[t_colorB].GetArray() );
				tex.scalar = static_cast<float>(texture[t_squareSize].GetDouble());
			} else if ( texture[t_type] == "bitmap" ) {
				tex.type = TextureType::Bitmap;
				assert( texture.HasMember( t_filePath ) && texture[t_filePath].IsString() );
				tex.filePath = std::string( "./rsc" ) + texture[t_filePath].GetString();

				int width, height, channels;
				//! stbi_set_flip_vertically_on_load( true ); // Optional: flip vertically
				unsigned char* image = stbi_load(
					tex.filePath.c_str(), &width, &height, &channels, 0 );
				assert( image != nullptr );
				tex.bitmap.width = width;
				tex.bitmap.height = height;
				tex.bitmap.channels = channels;
				tex.bitmap.buffer = image;
			} else {
				assert( false );
			}

			m_textures.push_back( tex );
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

		unsigned nrMats = matsArr.Size();
		m_materials.reserve( nrMats );
		for ( unsigned i{}; i < nrMats; ++i ) {
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
			else if ( matTypeStr == "refractive" )
				mat.type = MaterialType::Refractive;
			else if ( matTypeStr == "constant" )
				mat.type = MaterialType::Constant;

			mat.smoothShading = material[t_smShading].GetBool();

			// Assign a value for the IOR if there is one.
			if ( material.HasMember( t_ior ) && material[t_ior].IsDouble() )
				mat.ior = static_cast<float>(material[t_ior].GetDouble());

			// Assign a value for the albedo if there is one.
			if ( material.HasMember( t_albedo ) ) {
				if ( material[t_albedo].IsString() ) {
					mat.texName = material[t_albedo].GetString();
				} else if ( material[t_albedo].IsArray() ) {
					mat.texture.type = TextureType::SolidColor;
					mat.texture.albedo = loadVector3<Color>( material[t_albedo].GetArray() );
				}
			}

			for ( const Texture& tex : m_textures ) {
				if ( tex.name == mat.texName ) {
					mat.texture = tex;
				}
			}

			m_materials.push_back( mat );
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

std::vector<FVector3> loadUVs( const rapidjson::Value::ConstArray& arr ) {
	std::vector<FVector3> uvs{};

	for ( unsigned i{}; i + 2 < arr.Size(); i += 3 )
		uvs.emplace_back( static_cast<float>(arr[i].GetDouble()),
			static_cast<float>(arr[i + 1].GetDouble()),
			static_cast<float>(arr[i + 2].GetDouble()) );

	return uvs;
}

std::vector<FVector3> Scene::LoadVertices( const rapidjson::Value::ConstArray& arr ) {
	std::vector<FVector3> verts{};
	verts.reserve( arr.Size() / 3 );

	for ( unsigned i{}; i + 2 < arr.Size(); i += 3 ) {
		FVector3 vertex = {
			static_cast<float>(arr[i].GetDouble()),
			static_cast<float>(arr[i + 1].GetDouble()),
			static_cast<float>(arr[i + 2].GetDouble())
		};

		m_aabb.min.x = std::min( m_aabb.min.x, vertex.x );
		m_aabb.min.y = std::min( m_aabb.min.y, vertex.y );
		m_aabb.min.z = std::min( m_aabb.min.z, vertex.z );

		m_aabb.max.x = std::max( m_aabb.max.x, vertex.x );
		m_aabb.max.y = std::max( m_aabb.max.y, vertex.y );
		m_aabb.max.z = std::max( m_aabb.max.z, vertex.z );

		verts.push_back( std::move( vertex ) );
	}

	return verts;
}

void Scene::LoadMesh(
	const rapidjson::Value::ConstArray& arr,
	const std::vector<FVector3>& meshVerts,
	const std::vector<FVector3>& UVs,
	const int matIdx
) {
	std::vector<int> tris{};
	tris.reserve( arr.Size() );
	size_t vertSize{ meshVerts.size() };
	size_t trianglesInMesh{ arr.Size() / 3 };

	std::vector<FVector3> vertexNormals( vertSize, { 0.f, 0.f, 0.f } );
	std::vector<int> vertexNormalCounts( vertSize, 0 );

	// Create an override material for this mesh.
	Material overrideMat{};
	unsigned overrideMatIdx{};
	if ( settings.renderMode == RenderMode::RandomMeshColor ) {
		overrideMat.texture.albedo = getRandomColor();
		m_overrideMaterials.push_back( overrideMat );
		overrideMatIdx = static_cast<unsigned>(m_overrideMaterials.size()) - 1;
	}

	for ( unsigned i{}; i < arr.Size(); ++i ) {

		tris.emplace_back( arr[i].GetInt() );

		// Wait until 3 new vertices appear to create a triangle.
		if ( (i + 1) % 3 != 0 )
			continue;

		Vertex v0, v1, v2;
		int idx0 = tris[tris.size() - 3];
		int idx1 = tris[tris.size() - 2];
		int idx2 = tris[tris.size() - 1];

		v0.origIdx = idx0;
		v1.origIdx = idx1;
		v2.origIdx = idx2;

		v0.pos = meshVerts[idx0];
		v1.pos = meshVerts[idx1];
		v2.pos = meshVerts[idx2];

		if ( !UVs.empty() ) {
			v0.UVCoords = UVs[idx0];
			v1.UVCoords = UVs[idx1];
			v2.UVCoords = UVs[idx2];
		}
		v0.normal = v1.normal = v2.normal = {}; // Init normals to 0

		m_triangles.emplace_back( v0, v1, v2 );
		size_t lastTriIdx = m_triangles.size() - 1;
		m_triIndices.push_back( static_cast<int>(lastTriIdx) );

		vertexNormals[idx0] += m_triangles[lastTriIdx].GetNormal();
		vertexNormals[idx1] += m_triangles[lastTriIdx].GetNormal();
		vertexNormals[idx2] += m_triangles[lastTriIdx].GetNormal();

		vertexNormalCounts[idx0]++;
		vertexNormalCounts[idx1]++;
		vertexNormalCounts[idx2]++;

		if ( matIdx != -1 ) {
			m_triangles[lastTriIdx].matIdx = matIdx;
		}

		// Assign the override material index to each triangle
		if ( settings.renderMode == RenderMode::RandomMeshColor )
			m_triangles[lastTriIdx].overrideMatIdx = overrideMatIdx;

		if ( settings.renderMode == RenderMode::RandomTriangleColor ) {
			m_triangleColors.push_back( getRandomColor() );
			m_triangles[lastTriIdx].colorIdx = static_cast<unsigned>(
				m_triangleColors.size()) - 1;
		}
	}

	// Normalize all Vertex Normals.
	for ( size_t i{}; i < vertSize; ++i )
		if ( vertexNormalCounts[i] > 0 )
			vertexNormals[i].NormalizeInPlace();

	// Assign vertex normals to all vertices of this mesh.
	for ( size_t i{}; i < trianglesInMesh; ++i ) {
		Triangle& tri = m_triangles[m_triangles.size() - trianglesInMesh + i];
		tri.SetVertexNormal( 0u, vertexNormals[tri.GetVert( 0u ).origIdx] );
		tri.SetVertexNormal( 1u, vertexNormals[tri.GetVert( 1u ).origIdx] );
		tri.SetVertexNormal( 2u, vertexNormals[tri.GetVert( 2u ).origIdx] );
	}
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
		vertexIndices.push_back( std::stoi( vertexData.substr( 0, pos ) ) );
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
	settings.renderWidth = 1920;
	settings.renderHeight = 1080;
	settings.BGColor = Colors::Black;
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
}

const std::vector<Material>& Scene::GetMaterials() const {
	return m_materials;
}

const std::vector<Material>& Scene::GetOverrideMaterials() const {
	return m_overrideMaterials;
}

const std::vector<Color>& Scene::GetTriangleColors() const {
	return m_triangleColors;
}

const AccTree& Scene::GetAccTree() const {
	return m_accTree;
}

void Scene::SetRenderScene( const std::string& filePath ) {
	m_filePath = filePath;
}
