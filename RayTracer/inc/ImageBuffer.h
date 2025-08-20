#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <string> // string

#include "Colors.h" // Color


class ImageBuffer {
private:
	Color* data;
	unsigned m_width;
	unsigned m_height;

public:
	// @param[in] width: The width of the final image.
	// @param[in] height: The height of the final image.
	ImageBuffer( unsigned, unsigned );

	~ImageBuffer();

	// Copy constructor allocates new memory and copies the data.
	// @param[in] other: The ImageBuffer to copy from.
	ImageBuffer( const ImageBuffer& );

	// Copy assignment operator allocates new memory and copies the data.
	// @param[in] other: The ImageBuffer to copy from.
	ImageBuffer& operator=( const ImageBuffer& );

	// Get the width of the image.
	// @return The width as an unsigned int.
	unsigned GetWidth() const;

	// Get the height of the image.
	// @return The height as an unsigned int.
	unsigned GetHeight() const;

	// Load a PPM file into an ImageBuffer.
	// @param[in] path: The path to the PPM file to load.
	static ImageBuffer LoadPPM( const std::string& path );

	// Access operator for row-major order.
	// Allows syntax like: imageBuffer[rowIdx][colIdx]
	// @param[in] idx: The index of the row.
	// @return A pointer to the start of the specified row.
	Color* operator[]( const unsigned );

	// Const version of the access operator for read-only access.
	// @param[in] idx: The index of the row.
	// @return A pointer to the start of the specified row.
	const Color* operator[]( const unsigned ) const;
};


#endif // IMAGE_BUFFER_H