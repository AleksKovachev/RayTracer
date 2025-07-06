#include "Lights.h" // Light
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
	: m_fileName{ sceneFileName }, renderMode{ RenderMode::ShadedSmooth } {
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
	ParseMaterialsTag( doc );

	for ( Mesh& mesh : m_meshes ) {
		mesh.albedo = getRandomColor();
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
				m_meshes[i].matIdx = mesh[t_matIdx].GetInt();
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

	if ( doc.HasMember( t_materials ) && doc[t_materials].IsArray() ) {
		const rapidjson::Value::ConstArray& matsArr = doc[t_materials].GetArray();

		for ( unsigned i{}; i < matsArr.Size(); ++i ) {
			assert( matsArr[i].IsObject() );
			const rapidjson::Value& material{ matsArr[i] };
			assert( material.HasMember( t_type ) && material[t_type].IsString() );
			assert( material.HasMember( t_albedo ) && material[t_albedo].IsArray() );
			assert( material.HasMember( t_smShading ) && material[t_smShading].IsBool() );

			Material mat;
			if ( material[t_type].GetString() == "diffuse" )
				mat.type = MaterialType::Diffuse;
			else if ( material[t_type].GetString() == "reflective" )
				mat.type = MaterialType::Reflective;

			mat.albedo = loadVector3<Color>( material[t_albedo].GetArray() );
			mat.smoothShading = material[t_smShading].GetBool();

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
