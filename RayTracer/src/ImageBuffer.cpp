#include "ImageBuffer.h"

#include <fstream> // ifstream, ios::binary, streamsize::max
#include <limits> // numeric_limits
#include <stdexcept> // out_of_range, runtime_error
#include <cstdint> // uint8_t
#include <string> // string

ImageBuffer::ImageBuffer( const unsigned width, const unsigned height )
	: m_width( width ), m_height( height ) {
	// Allocates memory for the image buffer and initializes it.
	unsigned totalPixels{ width * height };
	data = new Color[totalPixels];

	for ( unsigned i{}; i < totalPixels; ++i )
		data[i] = Color{};
}

ImageBuffer::~ImageBuffer() {
	delete[] data;
}

ImageBuffer::ImageBuffer( const ImageBuffer& other )
	: m_width( other.m_width ), m_height( other.m_height ) {
	unsigned totalPixels{ m_width * m_height };
	data = new Color[totalPixels];

	for ( unsigned i{}; i < totalPixels; ++i )
		data[i] = other.data[i];
}

ImageBuffer& ImageBuffer::operator=( const ImageBuffer& other ) {
	if ( this == &other )
		return *this;

	// Clean up existing data
	delete[] data;

	// Copy dimensions
	m_width = other.m_width;
	m_height = other.m_height;

	// Allocate and copy pixel data
	unsigned totalPixels{ m_width * m_height };
	data = new Color[totalPixels];

	for ( unsigned i = 0; i < totalPixels; ++i )
		data[i] = other.data[i];

	return *this;
}

unsigned ImageBuffer::GetWidth() const {
	return m_width;
}

unsigned ImageBuffer::GetHeight() const {
	return m_height;
}

Color* ImageBuffer::operator[]( const unsigned idx ) {
	if ( idx >= m_height )
		throw std::out_of_range( "Row index out of bounds." );
	return data + (static_cast<size_t>(idx) * m_width);
}

const Color* ImageBuffer::operator[]( const unsigned idx ) const {
	if ( idx >= m_height )
		throw std::out_of_range( "Row index out of bounds." );
	return data + (static_cast<size_t>(idx) * m_width);
}

ImageBuffer ImageBuffer::LoadPPM( const std::string& path ) {
	std::ifstream file( path, std::ios::binary );
	if ( !file )
		throw std::runtime_error( "Failed to open file: " + path );

	std::string PPMFormat;
	file >> PPMFormat;
	if ( PPMFormat != "P6" )
		throw std::runtime_error( "Unsupported PPM format. Only binary P6 is supported." );

	unsigned width{};
	unsigned height{};
	unsigned maxVal{};

	// Skip comments and read width, height, maxVal
	while ( file.peek() == '#' )
		file.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

	file >> width >> height >> maxVal;

	// TODO: Fix with color depth.
	if ( maxVal != 255 )
		throw std::runtime_error( "Only maxVal of 255 is currently supported." );

	// Skip single whitespace character after header
	file.ignore( 1 );

	ImageBuffer buffer( width, height );

	for ( unsigned row{}; row < height; ++row ) {
		for ( unsigned col{}; col < width; ++col ) {
			uint8_t rgb[3]; // TODO: Change type with color depth introduction.
			file.read( reinterpret_cast<char*>( rgb ), 3 );
			if ( !file )
				throw std::runtime_error( "Unexpected end of file while reading pixel data." );
			buffer[row][col] = Color( rgb[0], rgb[1], rgb[2] );
		}
	}

	return buffer;
}
