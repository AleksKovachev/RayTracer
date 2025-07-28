#ifndef MATERIALS_H
#define MATERIALS_H

#include "Colors.h"


enum class MaterialType {
	Diffuse,
	Reflective
};

struct Material {
	MaterialType type;
	Color albedo;
	bool smoothShading;

	Material();
	Material( const MaterialType& matType, const Color& albedo, const bool smShading );
};

#endif // MATERIALS_H