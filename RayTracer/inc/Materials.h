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
	SolidColor, // Uses color settings: albedo, ior, etc.
	EdgesP, // Procedural texture to color triangle edges.
	CheckerP, // Procedural checker texture.
	Bitmap // Loading a bitmap image to use.
};


enum class NameType {
	Material,
	Texture
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
	static size_t counter; // A counter to add to default-created textures.
};


struct Material {
	MaterialType type;
	std::string texName; // Texture name.
	Texture texture; // Texture settings for this material.
	bool smoothShading; // If this material should be rendered smooth or flat.
	float ior; // Index of Refraction for refractive material type.
	static size_t counter; // A counter to add to default-created materials.

	// Default constructor initializes to a Diffuse type with an auto-generated name.
	Material();

	// @param[in] matType: The material type.
	// @param[in] texture: The texture object to go along with this material.
	// Reflection multiplier for reflective materials.
	// @param[in] smShading: Whether the material should be rendered smooth or flat.
	Material( const MaterialType&, const Texture&, const bool );
};

// Generates a default name.
// @param[in] type: The type of name to be generated.
// @return A string containing the name.
std::string GenerateDefaultName( NameType );

#endif // MATERIALS_H