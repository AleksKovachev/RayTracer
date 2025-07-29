#ifndef COLORUTILS_H
#define COLORUTILS_H

struct Color;

// Converts a normalized sRGB gamma encoded color value to a linearized value.
float sRGBtoLinearComponent( float );

// Converts a normalized sRGB gamma encoded color to a linearized color.
Color sRGBtoLinearColor( const Color& );

// Converts a linearized color value to a normalized sRGB gamma encoded value.
float lineartoSRGBComponent( float );

// Converts a linearized color to a normalized sRGB gamma encoded color.
Color lineartoSRGBColor( const Color& );

// Applies linear interpolation between two colors based on a factor t [0.0, 1.0].
Color lerpColor( const Color&, const Color&, const float );

float getLuminance_BT709( const Color& );

float getLuminance_HSP( const Color& );

float getLuminance_BT601( const Color& );

#endif // COLORUTILS_H