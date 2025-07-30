#include "ImageBuffer.h"

#include <stdexcept> // out_of_range

ImageBuffer::ImageBuffer( const unsigned width, const unsigned height )
	: m_width( width ), m_height( height ) {
	// Allocates memory for the image buffer and initializes it.
	unsigned totalPixels{ width * height };
	data = new Color[totalPixels];

	for ( unsigned i{}; i < totalPixels; ++i ) {
		data[i] = Color{};
	}
}

ImageBuffer::~ImageBuffer() {
	delete[] data;
}

unsigned ImageBuffer::GetWidth() const {
	return m_width;
}

unsigned ImageBuffer::GetHeight() const {
	return m_height;
}

Color* ImageBuffer::operator[]( const unsigned idx ) {
	if ( idx >= m_height ) {
		throw std::out_of_range( "Row index out of bounds." );
	}
	return data + (static_cast<size_t>(idx) * m_width);
}

const Color* ImageBuffer::operator[]( const unsigned idx ) const {
	if ( idx >= m_height ) {
		throw std::out_of_range( "Row index out of bounds." );
	}
	return data + (static_cast<size_t>(idx) * m_width);
}