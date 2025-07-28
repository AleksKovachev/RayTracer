#include "Colors.h"

#include <algorithm> // round
#include <iostream> // ostream
#include <iomanip> // setw

Color::Color( int in_r, int in_g, int in_b ) : r{ in_r }, g{ in_g }, b{ in_b } {}

Color::Color( float in_r, float in_g, float in_b )
    : r{ static_cast<int>(round( in_r * 255.f )) },
    g{ static_cast<int>(round( in_g * 255.f )) },
    b{ static_cast<int>(round( in_b * 255.f )) } {
}

Color& Color::operator=( const Color& other ) {
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

Color Color::operator/( const int val ) {
    return { static_cast<int>(roundf( static_cast<float>(r) / val )),
        static_cast<int>(roundf( static_cast<float>(g) / val )),
            static_cast<int>(roundf( static_cast<float>(b) / val )) };
}

Color& Color::operator+=( const Color& other ) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}
