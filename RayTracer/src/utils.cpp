#include "ColorUtils.h" // lineartoSRGBColor
#include "utils.h"

#include <algorithm> // max
#include <cassert> // assert
#include <cctype> // isspace
#include <fstream> // ifstream, ofstream
#include <iostream> // cerr
#include <string> // string, getline, npos, stof


int getInt( const int min, const int max ) {
	std::random_device seed;
	std::mt19937 gen( seed() );
	std::uniform_int_distribution<> genInt( min, max );

	return genInt( gen );
}

Color getRandomColor( const int colorDepth ) {
	const int maxComponent{ (1 << colorDepth) - 1 }; // 2^colorDepth
	return { getInt( 0, maxComponent ), getInt( 0, maxComponent ), getInt( 0, maxComponent ) };
}

void writeColorToFile(
	std::ofstream& stream, const Color& pixelColor, const int maxComp, bool outputSRGB
) {
	Color finalColor{};
	if ( outputSRGB )
		finalColor = lineartoSRGBColor( pixelColor );
	else
		finalColor = pixelColor;

	int R{ static_cast<int>(std::roundf( std::clamp( finalColor.r, 0.f, 1.f ) * maxComp )) };
	int G{ static_cast<int>(std::roundf( std::clamp( finalColor.g, 0.f, 1.f ) * maxComp )) };
	int B{ static_cast<int>(std::roundf( std::clamp( finalColor.b, 0.f, 1.f ) * maxComp )) };
	unsigned char r = static_cast<unsigned char>(R);
	unsigned char g = static_cast<unsigned char>(G);
	unsigned char b = static_cast<unsigned char>(B);
	stream.write( reinterpret_cast<const char*>(&r), 1 );
	stream.write( reinterpret_cast<const char*>(&g), 1 );
	stream.write( reinterpret_cast<const char*>(&b), 1 );
}

bool areEqual( const float a, const float b, const float epsilon ) {
	return std::fabs( a - b ) < epsilon;
}

bool isLT( const float a, const float b, const float epsilon ) {
	return (b - a) > epsilon;
}

bool isGT( const float a, const float b, const float epsilon ) {
	return (a - b) > epsilon;
}

bool isLE( const float a, const float b, const float epsilon ) {
	return (b - a) >= -epsilon;
}

bool isGE( const float a, const float b, const float epsilon ) {
	return (a - b) >= -epsilon;
}

bool areAlmostEqual( float a, float b, float epsilon ) {
	return std::fabs( a - b ) <= epsilon * std::max( std::fabs( a ), std::fabs( b ) );
}

float lerp( const float a, const float b, const float t ) {
	return a + t * (b - a);
}

iniData readConfig() {
	std::ifstream file( "./config/config.ini" );
	iniData values;

	assert( file.is_open() );

	std::string line;
	std::string varName;
	std::string initialValue;
	std::string valueNoComment;
	size_t commentStartIdx;
	std::string strValue;
	float fValue;
	//int iValue;

	while ( std::getline( file, line ) ) {
		if ( line.empty() )
			continue;
		// skip comments
		if ( line[0] == '#' )
			continue;
		size_t pos = line.find( '=' );
		if ( pos != std::string::npos ) {
			varName = line.substr( 0, pos );

			// If there's a variable with = and no value - skip it
			if ( pos + 1 < line.size() ) {
				initialValue = line.substr( pos + 1 );
				// Remove any comments after the value
				commentStartIdx = initialValue.find( '#' );
				valueNoComment = (commentStartIdx != std::string::npos) ?
					initialValue.substr( 0, commentStartIdx ) : initialValue;
				if ( valueNoComment != initialValue ) {
					for ( char ch : valueNoComment ) {
						if ( !std::isspace( static_cast<unsigned char>(ch) ) ) {
							strValue += ch;
						}
					}
				} else
					strValue = valueNoComment;
				try {
					fValue = std::stof( strValue );
				//!? Currently unused and unimplemented
				//} catch ( const std::invalid_argument e ) {
				//    iValue = std::stoi( strValue );
				} catch ( const std::invalid_argument e ) {
					std::cerr << "Invalid cast! Value: " <<
						strValue << " is not float or int!\n";
					std::exit( 1 );
				}
				values.insert( { varName, fValue } );
			}
		}
	}

	file.close();
	return values;
}

bool areCharsInString( const std::string& str, const std::unordered_set<char>* passedCharsPtr ) {
	if ( str.empty() )
		return false;

	const std::unordered_set<char>* charsSetPtr = passedCharsPtr;

	std::unordered_set<char> forbiddenChars;
	if ( passedCharsPtr == nullptr ) {
		forbiddenChars = { '<', '>', ':', '"', '/', '\\', '|', '?', '*' };
		forbiddenChars.reserve( 41 );
		for ( char c{}; c < 32; ++c )
			forbiddenChars.insert( c );
		charsSetPtr = &forbiddenChars;
	}

	for ( const char chStr : str )
		if ( charsSetPtr->count( chStr ) )
			return true;

	return false;
}