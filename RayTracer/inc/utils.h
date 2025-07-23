#ifndef UTILS_H
#define UTILS_H

#include <cmath> // fabs
#include <random> // random_device, mt19937, uniform_int_distribution
#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set

#include "Colors.h" // Color

class Scene;

typedef std::unordered_map<std::string, float> iniData;


// Get a random int in range[min, max].
// @param[in] min: The minimum number the random integer can be.
// @param[in] max: The maximum number the random integer can be.
// @return The randomly-generated integer number.
int getInt( const int, const int );

// Returns a random Color.
// @param[in] bitDepth: The color bit-depth. Defaults to 8-bit color.
// @return A randomly-generated color.
Color getRandomColor( const int bitDepth = 8 );

// Takes a stream and a Color to write to a PPM file in P6 format.
// @param[in-out] stream: The stream where the color data will be written.
// @param[in] pixelColor: The color data that will be written to the file.
// @param[in] maxComp: The maximum color component as an integer number.
void writeColorToFile( std::ofstream&, const Color&, const int );

// Check if 2 float values are equal with an epsilon range of error.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which the numbers are considered equal.
// Defaults to 1e-9.f.
// @return Boolean indication if the numbers are equal within the epsilon range.
bool areEqual( const float a, const float b, const float epsilon = 1e-9f );

// Check if float value A is less than B with an epsilon range of error.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which "a" is considered less than "b".
// Defaults to 1e-9.f.
// @return Boolean indication if "a" is less than "b" within the epsilon range.
bool isLT( const float a, const float b, const float epsilon = 1e-9f );

// Check if float value A is greater than B with an epsilon range of error.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which "a" is considered greater than "b".
// Defaults to 1e-9.f.
// @return Boolean indication if "a" is greater than "b" within the epsilon range.
bool isGT( const float a, const float b, const float epsilon = 1e-9f );

// Check if float value A is less than or equal to B with an epsilon range of error.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which "a" is considered to be
// less than or equal to "b". Defaults to 1e-9.f.
// @return Boolean indication if "a" is less than or equal to "b" within the epsilon range.
bool isLE( const float a, const float b, const float epsilon = 1e-9f );

// Check if float value A is greater than or equal to B with an epsilon range of error.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which "a" is considered to be
// greater than or equal to "b". Defaults to 1e-9.f.
// @return Boolean indication if "a" is greater than or equal to "b" within the epsilon range.
bool isGE( const float a, const float b, const float epsilon = 1e-9f );

// Relative Comparison for floats that vary greatly in magnitude.
// @param[in] a: The first number that will be compared.
// @param[in] b: The second number that will be compared.
// @param[in] epsilon: An error range, within which the numbers are considered equal.
// Defaults to 1e-9.f.
// @return Boolean indication if the numbers are equal within the epsilon range.
bool areAlmostEqual( float a, float b, float epsilon = 1e-9f );

// Reads a config.ini file for an improvised interactive rendering.
// @return An unordered map of values to use for different settings.
iniData readConfig();

// Checks if any of the provided characters is in the given string.
// If no characters are provided - checks for forbidden name characters.
// @param[in] std: The string to check for the provided characters.
// @param[in] chars: A pointer to a collection of characters to check if they're
// in the given string. Defaults to nullptr - checks for forbidden name characters.
// @return Boolean indication if any of the characters is in the string.
bool areCharsInString(
	const std::string& str, const std::unordered_set<char>* passedCharsPtr = nullptr);

#endif // UTILS_H