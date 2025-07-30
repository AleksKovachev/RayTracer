#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

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

	// Disable copy constructor and copy assignment operator
	ImageBuffer( const ImageBuffer& ) = delete;
	ImageBuffer& operator=( const ImageBuffer& ) = delete;

	// Get the width of the image.
	// @return The width as an unsigned int.
	unsigned GetWidth() const;

	// Get the height of the image.
	// @return The height as an unsigned int.
	unsigned GetHeight() const;

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