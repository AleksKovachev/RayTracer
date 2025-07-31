#ifndef ANTIALIASING_H
#define ANTIALIASING_H

#include "Colors.h" // Color


class ImageBuffer;
struct Settings;


enum class Antialiasing {
	NO, // No antialiasing
	FXAA // Fast Approximate Anti-Aliasing
};


class FXAA {
public:
	// @param[in] settings: A reference to a settings object.
	// @param[in] image: A pointer to an ImageBuffer object to be processed.
	// @param[in] linear: If the image paramter contains linear values or sRGB.
	FXAA( const Settings&, const ImageBuffer*, const bool );

	// Applies FXAA to the image.
	// @return: A pointer to a new ImageBuffer containing the processed image.
	// Pointer needs to be handled manually.
	const ImageBuffer* ApplyFXAA();
private:
	const Settings& m_settings; // Reference to the settings object.
	const unsigned m_width; // The width of the output image.
	const unsigned m_height; // The height of the output image.
	const ImageBuffer* m_image; // The image to be processed.
	bool inputIsLinear; // If the data in m_image holds linearized values.

	// Maximum distance to search for edge endpoints.[4,8].
	// Lower number = higher performance.
	static constexpr float SPAN_MAX = 8.f;
	// Absolute luminance contrast threshold [0.125, 0.166], 0.1 acceptable.
	// Lower values = more pixels processed.
	static constexpr float EDGE_THRESHOLD = 0.125f;
	// Relative luminance contrast threshold [0.03,0.063].
	// Lower values = more pixels processed.
	static constexpr float EDGE_THRESHOLD_MIN = 0.03f;
	// Controls edge scan detection [0.1-0.15].
	// Lower values = more aggressive edge detection.
	static constexpr float LUMA_THRESHOLD_FACTOR = 0.125f;
	// Multiplier for the edge strength to avoid over-blurring strong edges [~0.5].
	static constexpr float EDGE_STRENGTH_MUL = 0.5f;

	// Bilinear interpolation for a single channel.
	// @param[in] row: The index of the image row.
	// @param[in] col: The index of the image column.
	// @param[in] channel: The index of the color channel [0, 1, 2] -> [R, G, B].
	// @return: The interpolated value.
	float BilinearInterpolate( const float, const float, const int );

	// Function to get luminance from a pre-calculated luminance image
	// @param[in] luminanceImage: The pre-calculated luminance image.
	// @param[in] row: The index of the image row.
	// @param[in] col: The index of the image column.
	// @return: The luminance value at the calculated pixel.
	float GetLuminanceFromImage( const float*, int, int );

	// Applies FXAA to a single pixel.
	// @param[in] row: The index of the image row.
	// @param[in] col: The index of the image column.
	// @param[in] luminanceImage: The pre-calculated luminance image.
	// @return: A Color object if the processed color at the pixel.
	Color ApplyFXAAtoPixel( int, int, const float* );
};


#endif // ANTIALIASING_H