#ifndef UTILS_H
#define UTILS_H

#include <random>

// Return the normalized value based on a current and a max value.
template <typename T>
double normalize( T val, T max ) {
    return static_cast<double>(val) / max;
}

// Get a random int in range
int getInt( const int min, const int max ) {
    std::random_device seed;
    std::mt19937 gen( seed() );
    std::uniform_int_distribution<> genInt( min, max );

    return genInt( gen );
}

#endif // UTILS_H