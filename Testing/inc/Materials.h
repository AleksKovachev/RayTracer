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
};

#endif // MATERIALS_H