#include "utils.h"

#include <cmath>

template <typename T>
float normalize( T val, T max ) {
    return static_cast<float>(val) / max;
}

int getInt( const int min, const int max ) {
    std::random_device seed;
    std::mt19937 gen( seed() );
    std::uniform_int_distribution<> genInt( min, max );

    return genInt( gen );
}

Color getRandomColor( const int colorDepth ) {
    const int maxComponent{ (2 << colorDepth) - 1 };
    return { getInt( 0, maxComponent ), getInt( 0, maxComponent ), getInt( 0, maxComponent ) };
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
    return !(a - b > epsilon);
}

bool isGreaterEqualThan( const float a, const float b, const float epsilon) {
    return (b - a > epsilon);
}

bool areAlmostEqual( float a, float b, float epsilon) {
    return std::fabs( a - b ) <= epsilon * std::max( std::fabs( a ), std::fabs( b ) );
}