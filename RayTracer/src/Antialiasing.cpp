
#include "Antialiasing.h"
#include "ColorUtils.h" // sRGBtoLinearComponent, lineartoSRGBComponent, getLuminance
#include "ImageBuffer.h" // ImageBuffer
#include "RenderSettings.h" // Settings
#include "utils.h" // lerpColor

#include <algorithm> // clamp


FXAA::FXAA( const Settings& settings, const ImageBuffer* image, const bool linear )
	: m_settings{ settings },
	m_image{ image },
	m_width{ settings.renderWidth },
	m_height{ settings.renderHeight },
	inputIsLinear{ linear } {
	if ( m_image == nullptr )
		throw std::invalid_argument( "ImageBuffer pointer cannot be nullptr." );
}

const ImageBuffer* FXAA::ApplyFXAA() {
	if ( m_width == 0 || m_height == 0 )
		return m_image;

	ImageBuffer* processedImage = new ImageBuffer{ m_width, m_height };

	// Pre-calculate the luminance image
	float* luminanceImage = new float[m_width * m_height];

	for ( unsigned row{}; row < m_height; ++row ) {
		for ( unsigned col{}; col < m_width; ++col ) {
			Color pixelColor = (*m_image)[row][col];
			float luminance = getLuminance_BT601( pixelColor );
			luminanceImage[row * m_width + col] = luminance;
		}
	}

	// Apply FXAA using the pre-calculated luminance
	for ( unsigned row{}; row < m_settings.renderHeight; ++row ) {
		for ( unsigned col{}; col < m_settings.renderWidth; ++col ) {
			Color newColor = ApplyFXAAtoPixel( row, col, luminanceImage );
			(*processedImage)[row][col] = newColor;
		}
	}
	delete[] luminanceImage;
	return processedImage;
}

float FXAA::BilinearInterpolate( const float row, const float col, const int channel ) {
	// Skip floor as neither row, nor col can be negative.
	int row0 = static_cast<int>(row);
	int col0 = static_cast<int>(col);
	int row1 = row0 + 1;
	int col1 = col0 + 1;

	// Clamp coordinates to image boundaries.
	row0 = std::clamp( row0, 0, static_cast<int>(m_height) - 1 );
	col0 = std::clamp( col0, 0, static_cast<int>(m_width) - 1 );
	row1 = std::clamp( row1, 0, static_cast<int>(m_height) - 1 );
	col1 = std::clamp( col1, 0, static_cast<int>(m_width) - 1 );

	float fRow = row - row0;
	float fCol = col - col0;

	float c00{ (*m_image)[row0][col0][channel] };
	float c10{ (*m_image)[row1][col0][channel] };
	float c01{ (*m_image)[row0][col1][channel] };
	float c11{ (*m_image)[row1][col1][channel] };

	float interpolatedX0 = lerp( c00, c10, fRow );
	float interpolatedX1 = lerp( c01, c11, fRow );

	return lerp( interpolatedX0, interpolatedX1, fCol );
}

float FXAA::GetLuminanceFromImage(
	const float* luminanceImage, unsigned row, unsigned col ) {
	// Clamp coordinates to image boundaries.
	row = std::clamp( row, 0u, m_height - 1 );
	col = std::clamp( col, 0u, m_width - 1 );

	return luminanceImage[row * m_width + col];
}

Color FXAA::ApplyFXAAtoPixel(
	unsigned row,
	unsigned col,
	const float* luminanceImage
) {
	// Get center pixel luminance.
	float lumaCenter = GetLuminanceFromImage( luminanceImage, row, col );

	// Sample neighbors for luminance using a cross pattern to analyze gradients.
	float lumaTL = GetLuminanceFromImage( luminanceImage, row - 1u, col - 1u );
	float lumaTR = GetLuminanceFromImage( luminanceImage, row + 1u, col - 1u );
	float lumaBL = GetLuminanceFromImage( luminanceImage, row - 1u, col + 1u );
	float lumaBR = GetLuminanceFromImage( luminanceImage, row + 1u, col + 1u );

	float lumaMin = std::min( lumaCenter, std::min(
		std::min( lumaTL, lumaTR ), std::min( lumaBL, lumaBR ) ) );
	float lumaMax = std::max( lumaCenter, std::max(
		std::max( lumaTL, lumaTR ), std::max( lumaBL, lumaBR ) ) );

	float lumaRange = lumaMax - lumaMin;

	// Early exit if contrast is too low.
	const Color& origPixelColor = (*m_image)[row][col];
	if ( lumaRange < EDGE_THRESHOLD )
		return origPixelColor;
	if ( lumaRange < (lumaMax * EDGE_THRESHOLD_MIN) )
		return origPixelColor;

	// Sample neighbors for luminance using a cross pattern to analyze gradients.
	float lumaN = GetLuminanceFromImage( luminanceImage, row, col - 1u );
	float lumaS = GetLuminanceFromImage( luminanceImage, row, col + 1u );
	float lumaE = GetLuminanceFromImage( luminanceImage, row + 1u, col );
	float lumaW = GetLuminanceFromImage( luminanceImage, row - 1u, col );

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
		sampleCol = static_cast<float>( col ) + offset;
		sampleRow = static_cast<float>( row ); // X coordinate stays the same for horizontal edge.

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
		sampleRow = static_cast<float>( row ) + offset;
		sampleCol = static_cast<float>( col ); // Y coordinate doesn't change for vertical edge

		// Sample original image at new offset (X changes for vertical edge)
		// Using qRound for nearest neighbor. For better quality, implement bilinear.
		//Color sampledColor = *originalImage[static_cast<int>( std::round( row + offset ) )][col];

		//blendedColor = (*originalImage[row][col] + sampledColor) / 2;
	}

	Color sampledColor{
		BilinearInterpolate( sampleRow, sampleCol, 0 ),
		BilinearInterpolate( sampleRow, sampleCol, 1 ),
		BilinearInterpolate( sampleRow, sampleCol, 2 ) };

	float blendFactor{ std::clamp( lumaRange, 0.f, 1.f ) };

	Color blendedColor{ lerpColor( origPixelColor, sampledColor, blendFactor ) };

	return blendedColor;
}
