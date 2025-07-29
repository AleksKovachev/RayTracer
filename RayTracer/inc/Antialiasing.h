#ifndef ANTIALIASING_H
#define ANTIALIASING_H

#include "Colors.h" // Color


class ImageBuffer;
struct Settings;


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
	static constexpr float SPAN_MAX = 8.f; // Maximum distance to search for edge endpoints.
	static constexpr float REDUCE_MUL = 1.f / 8.f; // Multiplier for blend factor reduction.
	static constexpr float REDUCE_MIN = 1.f / 128.f; // Minimum blend factor reduction.
	static constexpr float EDGE_THRESHOLD = 0.166f; // Absolute luminance contrast threshold.
	static constexpr float EDGE_THRESHOLD_MIN = 0.063f; // Relative luminance contrast threshold.

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
	float GetLuminanceFromImage( const float*, unsigned, unsigned );

	// Applies FXAA to a single pixel.
	// @param[in] row: The index of the image row.
	// @param[in] col: The index of the image column.
	// @param[in] luminanceImage: The pre-calculated luminance image.
	// @return: A Color object if the processed color at the pixel.
	Color ApplyFXAAtoPixel( unsigned, unsigned, const float* );
};


#endif // ANTIALIASING_H