#include "Colors.h"

#include <algorithm> // clamp, roundf
#include <iostream> // ostream
#include <iomanip> // setw


Color::Color( const float in_r, const float in_g, const float in_b, const int depth )
    : r{ in_r }, g{ in_g }, b{ in_b } {
}

Color::Color( const int in_r, const int in_g, const int in_b, const int depth )
    : r{ static_cast<float> (in_r) / ((1 << depth) - 1) },
    g{ static_cast<float> (in_g) / ((1 << depth) - 1) },
    b{ static_cast<float> (in_b) / ((1 << depth) - 1) } {
}

float Color::GetCurrMaxComponent() {
    float maxComp = r;
    if ( g > r )
        maxComp = g;
    if ( b > maxComp )
        maxComp = b;
    return maxComp;
}

Color& Color::operator=( const Color& other ) {
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

bool Color::operator==( const Color& other ) const {
    return r == other.r && g == other.g && b == other.b;
}

Color Color::operator/( const float val ) const {
    return { r / val, g / val, b / val };
}

Color Color::operator*( const float val ) const {
    return { r * val, g * val, b * val };
}

Color Color::operator-( const Color& other ) const {
    return { r - other.r, g - other.g, b - other.b };
}

Color Color::operator+( const Color& other ) const {
    return { r + other.r, g + other.g, b + other.b };
}

Color& Color::operator+=( const Color& other ) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color& Color::operator-=( const Color& other ) {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
}

Color& Color::operator*=( const Color& other ) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    return *this;
}

Color& Color::operator/=( const int val ) {
    r /= val;
    g /= val;
    b /= val;
    return *this;
}
