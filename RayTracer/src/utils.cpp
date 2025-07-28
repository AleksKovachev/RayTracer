#include "utils.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>


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

void writeColorToFile( std::ofstream& stream, const Color& pixelColor ) {
    unsigned char r = static_cast<unsigned char>(pixelColor.r);
    unsigned char g = static_cast<unsigned char>(pixelColor.g);
    unsigned char b = static_cast<unsigned char>(pixelColor.b);
    stream.write( reinterpret_cast<const char*>(&r), 1 );
    stream.write( reinterpret_cast<const char*>(&g), 1 );
    stream.write( reinterpret_cast<const char*>(&b), 1 );
}

bool areEqual( const float a, const float b, const float epsilon) {
    return std::fabs( a - b ) < epsilon;
}

bool isLessThan( const float a, const float b, const float epsilon) {
    return (b - a) > epsilon;
}

bool isGreaterThan( const float a, const float b, const float epsilon) {
    return (a - b) > epsilon;
}

bool isLessEqualThan( const float a, const float b, const float epsilon) {
    return (b - a) >= -epsilon;
}

bool isGreaterEqualThan( const float a, const float b, const float epsilon) {
    return (a - b) >= -epsilon;
}

bool areAlmostEqual( float a, float b, float epsilon) {
    return std::fabs( a - b ) <= epsilon * std::max( std::fabs( a ), std::fabs( b ) );
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
                valueNoComment = (commentStartIdx != std::string::npos) ? initialValue.substr( 0, commentStartIdx ) : initialValue;
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

bool areCharsInString( const std::string& str, const std::vector<char>* passedChars ) {
    if ( !str.size() )
        return false;

    std::vector<char> chars{};

    if ( passedChars == nullptr ) {
        std::vector<char> forbiddenChars{ '<', '>', ':', '"', '/', '\\', '|', '?', '*' };
        forbiddenChars.reserve( 41 );
        for ( int i{}; i < 32; ++i )
            forbiddenChars.emplace_back( static_cast<char>(i) );
        chars = forbiddenChars;
    } else {
        chars = *passedChars;
    }

    for ( const char chStr : str ) {
        for ( const char chVec : chars ) {
            if ( chStr == chVec )
                return false;
        }
    }


    return true;
}