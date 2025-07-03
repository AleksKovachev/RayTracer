#include "Scene.h" // Scene, Mesh
#include "utils.h" // getRandomColor

#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <filesystem>


template <typename T>
T loadVector3( const rapidjson::Value::ConstArray& arr );
Matrix3 loadMatrix3( const rapidjson::Value::ConstArray& arr );
std::vector<FVector3> loadMeshVerts( const rapidjson::Value::ConstArray& arr );
std::vector<int> loadMeshTris( const rapidjson::Value::ConstArray& arr );


Scene::Scene( const std::string& sceneFileName )
	: m_fileName { sceneFileName } {
	std::filesystem::path path( m_fileName );
	m_settings.saveName = path.stem().string();
}

Scene::~Scene() {
	for ( Light* light : m_lights )
		delete light;
}


void Scene::ParseSceneFile() {
	std::ifstream ifs( m_fileName );
	assert( ifs.is_open() );

	rapidjson::IStreamWrapper isw( ifs );
	rapidjson::Document doc;
	doc.ParseStream( isw );
	assert( !doc.HasParseError() );

	ParseSettingsTag( doc );
	ParseCameraTag( doc );
	ParseObjectsTag( doc );
	ParseLightsTag( doc );

	for ( Mesh& mesh : m_meshes ) {
		mesh.color = getRandomColor();
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

	if ( doc.HasMember( t_objects ) && doc[t_objects].IsArray() ) {
		const rapidjson::Value::ConstArray& objArr = doc[t_objects].GetArray();

		for ( unsigned i{}; i < objArr.Size(); ++i ) {
			assert( objArr[i].IsObject() );
			const rapidjson::Value& mesh{ objArr[i] };
			assert( mesh.HasMember( t_vertices ) && mesh[t_vertices].IsArray() );
			assert( mesh.HasMember( t_triangles ) && mesh[t_triangles].IsArray() );
			m_meshes.emplace_back( loadMeshVerts( mesh[t_vertices].GetArray() ),
				loadMeshTris( mesh[t_triangles].GetArray() ) );
		}
	}
}

void Scene::ParseLightsTag( const rapidjson::Document& doc ) {
	// JSON Tags to look for
	char t_lights[]{ "lights" };
	char t_position[]{ "position" };

	if ( doc.HasMember( t_lights ) && doc[t_lights].IsArray() ) {
		const rapidjson::Value::ConstArray& lightsArr = doc[t_lights].GetArray();

		for ( unsigned i{}; i < lightsArr.Size(); ++i ) {
			assert( lightsArr[i].IsObject() );
			const rapidjson::Value& light{ lightsArr[i] };
			assert( light.HasMember(t_position) && light[t_position].IsArray());
			m_lights.emplace_back( loadVector3<FVector3>( light[t_position].GetArray() ) );
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
