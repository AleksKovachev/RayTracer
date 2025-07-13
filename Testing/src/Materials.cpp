#include "Materials.h"


Texture::Texture() : albedo{ Colors::Black } {}
Texture::Texture( const Color& albedo ) : albedo{ albedo } {}

size_t Material::counter = 0;

Material::Material()
	: type{ MaterialType::Diffuse },
	texType{ TextureType::ColorTexture },
	texName{ GenerateDefaultName() },
	texture{},
	smoothShading{ false },
	ior{ InvalidIOR } {
}

Material::Material( const MaterialType& matType, const Texture& texture, const bool smShading )
	: type{ matType },
	texType{ TextureType::ColorTexture },
	texName{ GenerateDefaultName() },
	texture{ texture },
	smoothShading{ smShading },
	ior{ InvalidIOR } {
}

std::string Material::GenerateDefaultName() {
	// Large enough for longest name "Color texture "
	// + largest_long_long_digits + null terminator.
	char buffer[64];

	if ( Material::counter < 10 )
		snprintf( buffer, sizeof( buffer ), "Color texture 00%lld", Material::counter++ );
	else if ( Material::counter < 100 )
		snprintf( buffer, sizeof( buffer ), "Color texture 0%lld", Material::counter++ );
	else
		snprintf( buffer, sizeof( buffer ), "Color texture 0%lld", Material::counter++ );
	return buffer;
}
