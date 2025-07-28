#include "Materials.h"

Material::Material()
	: type{ MaterialType::Diffuse }, albedo{ Colors::Black }, smoothShading{ false } {
}

Material::Material( const MaterialType& matType, const Color& albedo, const bool smShading )
	: type{ matType }, albedo{ albedo }, smoothShading{ smShading } {
}
