#include "Colors.h" // Colors::Black, Colors::Red
#include "Materials.h"

#include "stb_image.h" // stbi_image_free

#include <iostream> // cerr, endl

constexpr const char* INVALID = "<INVALID>";

Bitmap::Bitmap() : width{ 0 }, height{ 0 }, channels{ 0 }, buffer { nullptr } {}

size_t Texture::counter = 0;

Texture::Texture()
	: name{ GenerateDefaultName( NameType::Material ) },
	type{ TextureType::Invalid },
	albedo{ Colors::Black },
	colorA{ Colors::Red }, // using red as a warning color
	colorB{ Colors::Red }, // using red as a warning color
	scalar{ 0.0f },
	filePath{ INVALID },
	bitmap{} {
}

Texture::Texture( const Color& albedo )
	: name{ GenerateDefaultName( NameType::Texture ) },
	type{ TextureType::Invalid },
	albedo{ albedo },
	colorA{ Colors::Red }, // using red as a warning color
	colorB{ Colors::Red }, // using red as a warning color
	scalar{ 0.0f },
	filePath{ INVALID },
	bitmap{} {
}

size_t Material::counter = 0;

Material::Material()
	: type{ MaterialType::Diffuse },
	texName{ GenerateDefaultName( NameType::Material ) },
	texture{},
	smoothShading{ false },
	ior{ InvalidIOR } {
}

Material::Material( const MaterialType& matType, const Texture& texture, const bool smShading )
	: type{ matType },
	texName{ GenerateDefaultName( NameType::Texture ) },
	texture{ texture },
	smoothShading{ smShading },
	ior{ InvalidIOR } {
}

std::string GenerateDefaultName( NameType type ) {
	// Large enough for longest name "Color texture "
	// + largest_long_long_digits + null terminator.
	char buffer[64];

	size_t& counter{ type == NameType::Material ? Material::counter : Texture::counter };

	if ( counter < 10 )
		snprintf( buffer, sizeof( buffer ), "Color texture 00%lld", counter++ );
	else if ( Material::counter < 100 )
		snprintf( buffer, sizeof( buffer ), "Color texture 0%lld", counter++ );
	else
		snprintf( buffer, sizeof( buffer ), "Color texture 0%lld", counter++ );
	return buffer;
}

Bitmap::~Bitmap() {
	if ( buffer != nullptr )
		stbi_image_free( buffer );
}

// Copy constructor
Bitmap::Bitmap( const Bitmap& other )
	: width( other.width ),
	height( other.height ),
	channels( other.channels ),
	buffer( nullptr )
{
	if ( other.buffer != nullptr ) {
		size_t bufferSize = static_cast<size_t>(width) * height * channels;

		buffer = new unsigned char[bufferSize];

		if ( buffer ) { // Check for successful allocation
			std::memcpy( buffer, other.buffer, bufferSize );
		}
	}
}

// Move constructor
Bitmap::Bitmap( Bitmap&& other ) noexcept :
	width( other.width ),
	height( other.height ),
	channels( other.channels ),
	buffer( other.buffer ) // Steal the pointer
{
	// Nullify the source object's pointer so it doesn't free the memory
	other.buffer = nullptr;
}

// Copy assignment
Bitmap& Bitmap::operator=( const Bitmap& other ) {
	if ( this != &other ) {
		// Free existing buffer first, using stbi_image_free
		if ( buffer != nullptr ) {
			stbi_image_free( buffer );
			buffer = nullptr;
		}

		width = other.width;
		height = other.height;
		channels = other.channels;

		if ( other.buffer != nullptr ) {
			size_t bufferSize = static_cast<size_t>(width) * height * channels;

			buffer = new unsigned char[bufferSize];

			if ( buffer ) {
				std::memcpy( buffer, other.buffer, bufferSize );
			}
		}
		else {
			buffer = nullptr; // If source has no buffer, ensure current has none
		}
	}
	return *this;
}

// Move assignment
Bitmap& Bitmap::operator=( Bitmap&& other ) noexcept {
	if ( this != &other ) {
		// Free existing buffer first, using stbi_image_free
		if ( buffer != nullptr ) {
			stbi_image_free( buffer );
		}

		width = other.width;
		height = other.height;
		channels = other.channels;
		buffer = other.buffer; // Steal the pointer

		// Nullify the source object's pointer
		other.buffer = nullptr;
	}
	return *this;
}
