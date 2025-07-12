#ifndef MATERIALS_H
#define MATERIALS_H

#include "Colors.h" // Color, Colors::Black


enum class MaterialType {
	Diffuse,
	Reflective,
	Refractive,
	Constant
};

struct Material {
	MaterialType type;
	Color albedo; // Color for diffuse type, strength for reflection type.
	bool smoothShading; // If this material should be rendered smooth or flat.

	Material();
	// @param[in] matType: The material type.
	// @param[in] albedo: The albedo color of the material.
	// Reflection multiplier for reflective materials.
	// @param[in] smShading: Whether the material should be rendered smooth or flat.
	Material( const MaterialType&, const Color&, const bool );
};

#endif // MATERIALS_H