#include "Materials.h"

Material::Material()
	: type{ MaterialType::Diffuse }, albedo{ Colors::Black }, smoothShading{ true } {
}
