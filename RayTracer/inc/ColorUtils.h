#ifndef COLORUTILS_H
#define COLORUTILS_H

#include <cmath> //sqrtf

struct Color;

// Converts a normalized sRGB gamma encoded color value to a linearized value.
// @param[in] colorChannel: The color channel to linearize.
// @return The linearized value of the color channel.
float sRGBtoLinearComponent( float );

// Converts a normalized sRGB gamma encoded color to a linearized color.
// @param[in] color: The color to linearize.
// @return The linearized color.
Color sRGBtoLinearColor( const Color& );

// Converts a linearized color value to a normalized sRGB gamma encoded value.
// @param[in] colorChannel: The color channel to convert to sRGB.
// @return The converted sRGB value of the color channel.
float lineartoSRGBComponent( float );

// Converts a linearized color to a normalized sRGB gamma encoded color.
// @param[in] color: The color to convert to sRGB.
// @return The converted to sRGB color.
Color lineartoSRGBColor( const Color& );

// Applies linear interpolation between two colors based on a factor t [0.0, 1.0].
// @param[in] colorA: The first color to interpolate.
// @param[in] colorB: The second color to interpolate.
// @param[in] t: The interpolation value.
// @return: The new interpolated color.
Color lerpColor( const Color&, const Color&, const float );

// Gets the luminance value of a color using BT709 format. Expects linearized values.
// @param[in] color: The normalized color to get the luminance of.
// @return The luminance value.
float getLuminance_BT709( const Color& );

// Gets the luminance value of a color using HSP format (unofficial). Expects linearized values.
// @param[in] color: The normalized color to get the luminance of.
// @return The luminance value.
float getLuminance_HSP( const Color& );

// Gets the luminance value of a color using BT601 format. Expects linearized values.
// @param[in] color: The normalized color to get the luminance of.
// @return The luminance value.
float getLuminance_BT601( const Color& );

// Calculates Euclidean distance between two colors.
// @param[in] a: The first color to compare.
// @param[in] b: The second color to compare.
// @return The Euclidean distance between colors A and B.
float getColorDifference( const Color& a, const Color& b );

#endif // COLORUTILS_H