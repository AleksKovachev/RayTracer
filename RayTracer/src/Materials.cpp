#include "Materials.h"


Material::Material()
	: type{ MaterialType::Diffuse },
	albedo{ Colors::Black },
	smoothShading{ false },
	ior{ InvalidIOR } {
}

Material::Material( const MaterialType& matType, const Color& albedo, const bool smShading )
	: type{ matType }, albedo{ albedo }, smoothShading{ smShading }, ior{ InvalidIOR } {
}
