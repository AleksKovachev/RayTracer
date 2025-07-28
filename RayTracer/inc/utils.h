#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <random>
#include <unordered_map>

#include "Bases.h"

typedef std::unordered_map<std::string, float> iniData;

// Return the normalized value based on a current and a max value.
template <typename T>
float normalize( T val, T max );

// Get a random int in range
int getInt( const int min, const int max );

// Returns a random Color
Color getRandomColor( const int bitDepth = 8 );

// Check if 2 float values are equal with an epsilon range of error
bool areEqual( const float a, const float b, const float epsilon = 1e-9 );

// Check if float value A is less than B with an epsilon range of error
bool isLessThan( const float a, const float b, const float epsilon = 1e-9 );

// Check if float value A is greater than B with an epsilon range of error
bool isGreaterThan( const float a, const float b, const float epsilon = 1e-9 );

// Check if float value A is less than or equal to B with an epsilon range of error
bool isLessEqualThan( const float a, const float b, const float epsilon = 1e-9 );

// Check if float value A is greater than or equal to B with an epsilon range of error
bool isGreaterEqualThan( const float a, const float b, const float epsilon = 1e-9 );

// Relative Comparison for floats that vary greatly in magnitude
bool areAlmostEqual( float a, float b, float epsilon = 1e-9 );

iniData readConfig();

#endif // UTILS_H