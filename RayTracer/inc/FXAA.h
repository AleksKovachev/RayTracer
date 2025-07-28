#ifndef FXAA_H
#define FXAA_H

#include <algorithm> // clamp
#include <cmath> // sqrt, pow

#include "Colors.h"
#include "ImageBuffer.h" // ImageBuffer
#include "RenderSettings.h" // Settings

constexpr float SPAN_MAX = 8.f; // Maximum distance to search for edge endpoints.
constexpr float REDUCE_MUL = 1.f / 8.f; // Multiplier for blend factor reduction.
constexpr float REDUCE_MIN = 1.f / 128.f; // Minimum blend factor reduction.
constexpr float EDGE_THRESHOLD = 0.166f; // Absolute luminance contrast threshold.
constexpr float EDGE_THRESHOLD_MIN = 0.063f; // Relative luminance contrast threshold.


float sRGBtoLinear( float colorChannel ) {
	// Converts a normalized sRGB gamma encoded color value to a linearized value.
	if ( colorChannel <= 0.04045f )
		return colorChannel / 12.92f;
	else
		return std::pow( ((colorChannel + 0.055f) / 1.055f), 2.4f );
}

float LineartosRGB( float colorChannel ) {
	// Converts a linearized color value to a normalized sRGB gamma encoded value.
	if ( colorChannel <= 0.0031308f )
		return 12.92f * colorChannel;
	else
		return 1.055f * std::pow( colorChannel, 1.f / 2.4f ) - 0.055f;
}

float getLuminance( const Color& color ) {
	// ITU BT.709
	//return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
	// HSP
	//std::sqrt( 0.299 * color.r * color.r + 0.587 * color.g * color.g + 0.114 * color.b * color.b );
	// ITU BT.601
	return 0.299f * sRGBtoLinear( color.r )
		 + 0.587f * sRGBtoLinear( color.g )
		 + 0.114f * sRGBtoLinear( color.b );
}

// Linear interpolation
float lerp( float a, float b, float t ) {
	return a + t * (b - a);
}

// Bilinear interpolation for a single channel.
float bilinearInterpolate(
	const Settings& settings, const ImageBuffer& image, float row, float col, int channel ) {
	// Skip floor as neither row, nor col can be negative.
	int row0 = static_cast<int>(row);
	int col0 = static_cast<int>(col);
	int row1 = row0 + 1;
	int col1 = col0 + 1;

	// Clamp coordinates to image boundaries.
	row0 = std::clamp( row0, 0, static_cast<int>(settings.renderHeight) - 1 );
	col0 = std::clamp( col0, 0, static_cast<int>(settings.renderWidth) - 1 );
	row1 = std::clamp( row1, 0, static_cast<int>(settings.renderHeight) - 1 );
	col1 = std::clamp( col1, 0, static_cast<int>(settings.renderWidth) - 1 );

	float fRow = row - row0;
	float fCol = col - col0;

	float c00{};
	float c10{};
	float c01{};
	float c11{};
	if ( channel == 0 ) {
	    c00 = image[row0][col0].r;
	    c10 = image[row1][col0].r;
	    c01 = image[row0][col1].r;
	    c11 = image[row1][col1].r;
	}
	else if ( channel == 1 ) {
	    c00 = image[row0][col0].g;
	    c10 = image[row1][col0].g;
	    c01 = image[row0][col1].g;
	    c11 = image[row1][col1].g;
	}
	else {
	    c00 = image[row0][col0].b;
	    c10 = image[row1][col0].b;
	    c01 = image[row0][col1].b;
	    c11 = image[row1][col1].b;
	}


	float interpolatedX0 = lerp( c00, c10, fRow );
	float interpolatedX1 = lerp( c01, c11, fRow );

	return lerp( interpolatedX0, interpolatedX1, fCol );
}

// Function to get luminance from a pre-calculated luminance image, where 
// luminance is stored in the R channel of each pixel.
float getLuminanceFromImage(
	const ImageBuffer& luminanceImage, unsigned row, unsigned col, const Settings& settings ) {
	// Clamp coordinates to image boundaries.
	row = std::clamp( row, 0u, settings.renderHeight - 1 );
	col = std::clamp( col, 0u, settings.renderWidth - 1 );

	return luminanceImage[row][col].r;
}

// Applies FXAA to a single pixel.
Color applyFXAAtoPixel(
	const ImageBuffer* originalImage,
	unsigned row,
	unsigned col,
	const ImageBuffer& luminanceImage,
	const Settings& settings
) {
	// Get center pixel luminance.
	float lumaCenter = getLuminanceFromImage( luminanceImage, row, col, settings );

	// Sample neighbors for luminance using a cross pattern to analyze gradients.
	float lumaTL = getLuminanceFromImage( luminanceImage, row - 1u, col - 1u, settings );
	float lumaTR = getLuminanceFromImage( luminanceImage, row + 1u, col - 1u, settings );
	float lumaBL = getLuminanceFromImage( luminanceImage, row - 1u, col + 1u, settings );
	float lumaBR = getLuminanceFromImage( luminanceImage, row + 1u, col + 1u, settings );

	float lumaMin = std::min( lumaCenter, std::min(
		std::min( lumaTL, lumaTR ), std::min( lumaBL, lumaBR ) ) );
	float lumaMax = std::max( lumaCenter, std::max(
		std::max( lumaTL, lumaTR ), std::max( lumaBL, lumaBR ) ) );

	float lumaRange = lumaMax - lumaMin;

	// Early exit if contrast is too low.
	const Color& origPixelColor = (*originalImage)[row][col];
	if ( lumaRange < EDGE_THRESHOLD )
		return origPixelColor;
	if ( lumaRange < (lumaMax * EDGE_THRESHOLD_MIN) )
		return origPixelColor;

	// Sample neighbors for luminance using a cross pattern to analyze gradients.
	float lumaN = getLuminanceFromImage( luminanceImage, row, col - 1u, settings );
	float lumaS = getLuminanceFromImage( luminanceImage, row, col + 1u, settings );
	float lumaE = getLuminanceFromImage( luminanceImage, row + 1u, col, settings );
	float lumaW = getLuminanceFromImage( luminanceImage, row - 1u, col, settings );

	// Determine edge direction (simplified: horizontal or vertical based on gradients).
	float gradX = std::abs( lumaE - lumaW );
	float gradY = std::abs( lumaS - lumaN );

	bool horizontal = gradX >= gradY;

	// Calculate blend factor and subpixel offset.
	float offset = 0.f;
	float sampleCol{};
	float sampleRow{};

	if ( horizontal ) {
		// Average of north and south neighbors for horizontal edge.
		float lumaAvg = (lumaN + lumaS) * 0.5f;

		// Determine offset direction
		if ( lumaAvg < lumaCenter )
			offset = 1.0f; // Sample "down".
		else
			offset = -1.0f; // Sample "up".

		// Apply blend factor
		offset *= std::max( REDUCE_MIN, lumaRange * REDUCE_MUL );

		// Calculate exact sample position
		sampleCol = col + offset;
		sampleRow = row; // X coordinate stays the same for horizontal edge.

		// Sample original image at new offset (Y changes for horizontal edge)
		// Using qRound for nearest neighbor. For better quality, implement bilinear.
		//Color sampledColor = *originalImage[row][static_cast<int>(std::round( col + offset ))];

		// Simplified averaging. Real FXAA uses more complex weighting.
		//blendedColor = (*originalImage[row][col] + sampledColor) / 2.f;


	} else { // Vertical edge
		// Average of east and west neighbors for vertical edge.
		float lumaAvg = (lumaE + lumaW) * 0.5f;

		// Determine offset direction
		if ( lumaAvg < lumaCenter )
			offset = 1.0f; // Sample "right"
		else
			offset = -1.0f; // Sample "left"

		offset *= std::max( REDUCE_MIN, lumaRange * REDUCE_MUL );

		// Calculate exact sample position
		sampleRow = row + offset;
		sampleCol = col; // Y coordinate doesn't change for vertical edge

		// Sample original image at new offset (X changes for vertical edge)
		// Using qRound for nearest neighbor. For better quality, implement bilinear.
		//Color sampledColor = *originalImage[static_cast<int>( std::round( row + offset ) )][col];

		//blendedColor = (*originalImage[row][col] + sampledColor) / 2;
	}

	Color blendedColor{
		bilinearInterpolate( settings, *originalImage, sampleRow, sampleCol, 0 ),
		bilinearInterpolate( settings, *originalImage, sampleRow, sampleCol, 1 ),
		bilinearInterpolate( settings, *originalImage, sampleRow, sampleCol, 2 ) };
	return blendedColor;
}

const ImageBuffer* applyFxaatoImage( const ImageBuffer* originalImage, const Settings& settings ) {
	if ( originalImage == nullptr || settings.renderWidth == 0 || settings.renderHeight == 0 )
		return originalImage;

	ImageBuffer* processedImage = new ImageBuffer{ settings.renderWidth, settings.renderHeight };

	// Pre-calculate the luminance image
	ImageBuffer precalculatedLuminanceImage{ settings.renderWidth, settings.renderHeight };

	for ( unsigned row{}; row < settings.renderHeight; ++row ) {
		for ( unsigned col{}; col < settings.renderWidth; ++col ) {
			Color pixelColor = *originalImage[row][col];
			float luminance = getLuminance( pixelColor );
			precalculatedLuminanceImage[row][col] = Color(luminance, 0.f, 0.f );
		}
	}

	// Apply FXAA using the pre-calculated luminance
	for ( unsigned row{}; row < settings.renderHeight; ++row ) {
		for ( unsigned col{}; col < settings.renderWidth; ++col ) {
			Color newColor = applyFXAAtoPixel(
				originalImage, row, col, precalculatedLuminanceImage, settings );
			(*processedImage)[row][col] = newColor;
		}
	}

	return processedImage;
}

#endif // FXAA_H