#ifndef MATERIALS_H
#define MATERIALS_H

#include <limits> // min
#include <string> // string

#include "Colors.h" // Color, Colors::Black

constexpr float InvalidIOR = std::numeric_limits<float>::quiet_NaN();

enum class MaterialType {
	Diffuse,
	Reflective,
	Refractive,
	Constant
};

enum class TextureType {
	Invalid, // Hasn't been set.
	ColorTexture, // Uses color settings: albedo, ior, etc.
	RedGreenEdgesP, // Procedural texture to color triangle edges.
	BlackWhiteCheckerP, // Procedural checker texture.
	Bitmap // Loading a bitmap image to use.
};

class Bitmap {
public:
	int width;
	int height;
	int channels;
	unsigned char* buffer = nullptr;

	Bitmap();
	// Copy constructor
	Bitmap( const Bitmap& other );
	// Move constructor
	Bitmap( Bitmap&& other ) noexcept;
	// Copy assignment
	Bitmap& operator=( const Bitmap& other );
	// Move assignment
	Bitmap& operator=( Bitmap&& other ) noexcept;

	~Bitmap();
};

// A general Texture type incorporating all texture sub-types. Avoids polymorphism.
struct Texture {
	Texture();
	Texture( const Color& albedo );

	std::string name; // Texture name.
	TextureType type; // Texture type.
	Color albedo; // Color for diffuse type, strength for reflection type.
	Color colorA; // Primary color used for procedural textures.
	Color colorB; // Secondary color used for procedural textures.
	float scalar; // A scalar multiplier used for edge width, square size, etc.
	// File path leading to the texture location relative to the rsc directory.
	std::string filePath;
	Bitmap bitmap;
};

struct Material {
	MaterialType type;
	TextureType texType; // Texture type.
	std::string texName; // Texture name.
	Texture texture; // Texture settings for this material.
	bool smoothShading; // If this material should be rendered smooth or flat.
	float ior; // Index of Refraction for refractive material type.
	static size_t counter; // A counter to add to default-created materials.

	Material();
	// @param[in] matType: The material type.
	// @param[in] texture: The texture object to go along with this material.
	// Reflection multiplier for reflective materials.
	// @param[in] smShading: Whether the material should be rendered smooth or flat.
	Material( const MaterialType&, const Texture&, const bool );

	static std::string GenerateDefaultName();
};

#endif // MATERIALS_H