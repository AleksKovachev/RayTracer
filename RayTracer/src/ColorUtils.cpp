#include "Colors.h" // Color
#include "ColorUtils.h"
#include "utils.h" // lerp

#include <cmath> // pow, sqrtf

float sRGBtoLinearComponent( float colorChannel ) {
	if ( colorChannel <= 0.04045f )
		return colorChannel / 12.92f;
	else
		return std::powf( ((colorChannel + 0.055f) / 1.055f), 2.4f );
}

Color sRGBtoLinearColor( const Color& color ) {
	return {
		sRGBtoLinearComponent( color.r ),
		sRGBtoLinearComponent( color.g ),
		sRGBtoLinearComponent( color.b )
	};
}

float lineartoSRGBComponent( float colorChannel ) {
	if ( colorChannel <= 0.0031308f )
		return 12.92f * colorChannel;
	else
		return 1.055f * std::powf( colorChannel, 1.f / 2.4f ) - 0.055f;
}

Color lineartoSRGBColor( const Color& color ) {
	return {
		lineartoSRGBComponent( color.r ),
		lineartoSRGBComponent( color.g ),
		lineartoSRGBComponent( color.b )
	};
}

Color lerpColor( const Color& colorA, const Color& colorB, const float t ) {
	return {
		lerp( colorA.r, colorB.r, t ),
		lerp( colorA.g, colorB.g, t ),
		lerp( colorA.b, colorB.b, t )
	};
}

float getLuminance_BT7009( const Color& color ) {
	return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
}

float getLuminance_HSP( const Color& color ) {
	return std::sqrtf(
		0.299f * color.r * color.r + 0.587f * color.g * color.g + 0.114f * color.b * color.b );
}

float getLuminance_BT601( const Color& color ) {
	return 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
}