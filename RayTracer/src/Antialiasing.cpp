
#include "Antialiasing.h"
#include "ColorUtils.h" // sRGBtoLinearComponent, lineartoSRGBComponent, getLuminance
#include "ImageBuffer.h" // ImageBuffer
#include "RenderSettings.h" // Settings
#include "utils.h" // lerpColor

#include <algorithm> // clamp
#include <cmath> // fabs


FXAA::FXAA(
	const Settings& settings,
	const ImageBuffer* image,
	const bool linear
)
	: m_settings{ settings },
	m_image{ image },
	m_width{ settings.renderWidth },
	m_height{ settings.renderHeight },
	inputIsLinear{ linear },
	m_edgeDetection{ settings.edgeDetectionTypeFXAA } {
	if ( m_image == nullptr )
		throw std::invalid_argument( "ImageBuffer pointer cannot be nullptr." );
}

const ImageBuffer* FXAA::ApplyFXAA() {
	if ( m_width == 0 || m_height == 0 )
		return m_image;

	ImageBuffer* processedImage = new ImageBuffer{ m_width, m_height };

	// Pre-calculate the luminance image
	float* luminanceImage = new float[m_width * m_height];

	for ( int row{}; row < static_cast<int>( m_height ); ++row ) {
		for ( int col{}; col < static_cast<int>( m_width ); ++col ) {
			Color pixelColor = (*m_image)[row][col];
			float luminance = getLuminance_BT601( pixelColor );
			luminanceImage[row * m_width + col] = luminance;
		}
	}

	// Apply FXAA using the pre-calculated luminance
	for ( int row{}; row < static_cast<int>( m_height ); ++row ) {
		for ( int col{}; col < static_cast<int>( m_width ); ++col ) {
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
	const float* luminanceImage, int row, int col ) {
	// Clamp coordinates to image boundaries.
	row = std::clamp( row, 0, static_cast<int>(m_height) - 1 );
	col = std::clamp( col, 0, static_cast<int>(m_width) - 1 );

	return luminanceImage[row * m_width + col];
}

Color FXAA::ApplyFXAAtoPixel( int row, int col, const float* luminanceImage ) {
	// Get center pixel luminance.
	float lumaCenter = GetLuminanceFromImage( luminanceImage, row, col );

	// Sample neighbors for luminance using a cross pattern to analyze gradients.
	float lumaN = GetLuminanceFromImage( luminanceImage, row - 1, col );
	float lumaNE = GetLuminanceFromImage( luminanceImage, row - 1, col + 1 );
	float lumaE = GetLuminanceFromImage( luminanceImage, row, col + 1 );
	float lumaSE = GetLuminanceFromImage( luminanceImage, row + 1, col + 1 );
	float lumaS = GetLuminanceFromImage( luminanceImage, row + 1, col );
	float lumaSW = GetLuminanceFromImage( luminanceImage, row + 1, col - 1 );
	float lumaW = GetLuminanceFromImage( luminanceImage, row, col - 1 );
	float lumaNW = GetLuminanceFromImage( luminanceImage, row - 1, col - 1 );

	float lumaMin = std::min(
		{ lumaCenter, lumaN, lumaNE, lumaE, lumaSE, lumaS, lumaSW, lumaW, lumaNW } );
	float lumaMax = std::max(
		{ lumaCenter, lumaN, lumaNE, lumaE, lumaSE, lumaS, lumaSW, lumaW, lumaNW } );

	float lumaRange = lumaMax - lumaMin;

	// Early exit if contrast is too low.
	const Color& centerColor = (*m_image)[row][col];
	if ( lumaRange < EDGE_THRESHOLD )
		return centerColor;
	if ( lumaRange < (lumaMax * EDGE_THRESHOLD_MIN) )
		return centerColor;

	float colorDiffMax{};
	const unsigned neighborSamples{ 8 };
	if ( m_edgeDetection == EdgeDetection::CHROMA || m_edgeDetection == EdgeDetection::COMBINED ) {
		const int safeNegRow{ std::max( row - 1, 0 ) };
		const int safePosRow{ std::min( row + 1, static_cast<int>( m_height ) ) };
		const int safeNegCol{ std::max( col - 1, 0 ) };
		const int safePosCol{ std::min( col + 1, static_cast<int>( m_width ) ) };
		const Color* neighborColors[neighborSamples]{
			&(*m_image)[safeNegRow][col],        // N
			&(*m_image)[safeNegRow][safePosCol], // NE
			&(*m_image)[row][safePosCol],        // E
			&(*m_image)[safePosRow][safePosCol], // SE
			&(*m_image)[safePosRow][col],        // S
			&(*m_image)[safePosRow][safeNegCol], // SW
			&(*m_image)[row][safeNegCol],        // W
			&(*m_image)[safeNegRow][safeNegCol]  // NW
		};
		for ( int i{}; i < 8; ++i ) {
			float diff = getColorDifference( centerColor, (*neighborColors)[i] );
			if ( diff > colorDiffMax )
				colorDiffMax = diff;
		}
	}

	// Compute blend factor based on local contrast and edge span.
	float edgeStrengthLuma{ std::fabs(
		lumaCenter - 0.5f * (lumaMin + lumaMax) ) / (lumaMax - lumaMin + 1e-6f) };

	float edgeStrength{};
	switch ( m_edgeDetection ) {
		case EdgeDetection::LUMA:
			edgeStrength = edgeStrengthLuma;
			break;
		case EdgeDetection::CHROMA:
			edgeStrength = colorDiffMax / 1.732f; // sqrtf( 3.f );
			break;
		case EdgeDetection::COMBINED:
			float maxColorDist{ colorDiffMax / 1.732f }; // sqrtf( 3.f );
			edgeStrength = std::max( edgeStrengthLuma, CHROMA_LUMA_WEIGHT * maxColorDist );
			break;
	}

	// Early exit if strength is too low.
	if ( edgeStrength < EDGE_THRESHOLD )
		return centerColor;

	// Determine edge orientation (horizontal or vertical based on gradients).
	float gradHor = std::fabs( lumaW - lumaE );
	float gradVer = std::fabs( lumaN - lumaS );
	bool horizontal = gradHor >= gradVer;
	int offX{ horizontal ? 1 : 0 };
	int offY{ horizontal ? 0 : 1 };

	float lumaThreshold{ LUMA_THRESHOLD_FACTOR * (lumaMax - lumaMin) };

	int neg{};
	int pos{};

	while ( neg > -SPAN_MAX ) {
		int cx = col - offX * (-neg);
		int cy = row - offY * (-neg);
		float luma = GetLuminanceFromImage( luminanceImage, cy, cx );
		if ( std::fabs( luma - lumaCenter ) > lumaThreshold )
			break;
		--neg;
	}
	while ( pos < SPAN_MAX ) {
		int cx = col + offX * (pos + 1);
		int cy = row + offY * (pos + 1);
		float luma = GetLuminanceFromImage( luminanceImage, cy, cx );
		if ( std::fabs( luma - lumaCenter ) > lumaThreshold )
			break;
		++pos;
	}

	// Compute sub-pixel offset.
	float span{ static_cast<float>(neg + pos) };
	// Normalize to [-0.5, 0.f].
	float subpixelOffset{ (span == 0 ? 0 : static_cast<float>(-neg) / span) - 0.5f };

	float sampleCol{};
	float sampleRow{};

	if ( horizontal ) {
		// Calculate exact sample position
		sampleCol = static_cast<float>(col) + subpixelOffset;
		sampleRow = static_cast<float>(row); // X coordinate stays the same for horizontal edge.
	} else { // Vertical edge
		// Calculate exact sample position
		sampleRow = static_cast<float>(row) + subpixelOffset;
		sampleCol = static_cast<float>(col); // Y coordinate doesn't change for vertical edge.
	}

	Color sampledColor{
		BilinearInterpolate( sampleRow, sampleCol, 0 ),
		BilinearInterpolate( sampleRow, sampleCol, 1 ),
		BilinearInterpolate( sampleRow, sampleCol, 2 ) };

	// blendFactor scales edgeStrength to control over-blurring strong edges.
	float blendFactor{ std::clamp( edgeStrength * EDGE_STRENGTH_MUL, 0.0f, 1.0f ) };

	return lerpColor( centerColor, sampledColor, blendFactor );
}
