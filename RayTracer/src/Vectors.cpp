#include "Vectors.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>

Vector2 Vector2::operator+( const Vector2& val ) const {
	return Vector2( x + val.x, y + val.y );
}

FVector2 FVector2::operator+( const double val ) const {
	return FVector2( x + val, y + val );
}

FVector2 FVector2::operator+( const FVector2& val ) const {
	return FVector2( x + val.x, y + val.y );
}

Vector3 Vector3::operator+( const Vector3& other ) const {
	return { x + other.x, y + other.y, z + other.z };
}

Vector3& Vector3::operator+=( const Vector3& other ) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

void FVector3::init() {
	setLength();
}

FVector3 FVector3::operator+( const FVector3& other ) const {
	return { x + other.x, y + other.y, z + other.z };
}

FVector3 FVector3::operator-( const FVector3& other ) const {
	return { x - other.x, y - other.y, z - other.z };
}

FVector3& FVector3::operator+=( const FVector3& other ) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// Cross product
FVector3 FVector3::operator*( const FVector3& other ) {
	return { y * other.z - z * other.y,
			 z * other.x - x * other.z,
			 x * other.y - y * other.x };
}

// Scalar multiplication
FVector3 FVector3::operator*( const double& other ) {
	return { x * other, y * other, z * other };
}


double FVector3::getLength() const {
	return length;
}

FVector3 FVector3::normalize() {
	return { x / length, y / length, z / length };
}

void FVector3::normalizeInPlace() {
	double vecLen{ sqrt( x * x + y * y + z * z ) };
	x /= vecLen;
	y /= vecLen;
	z /= vecLen;
}

void FVector3::setLength() {
	length = sqrt( x * x + y * y + z * z );
}

// Easy printing of a Vector2
std::ostream& operator<<( std::ostream& os, const Vector2& vec ) {
	const size_t maxLen{ std::max(
		std::to_string( vec.x ).length(),
		std::to_string( vec.y ).length()
	) };
	os << "(" << std::setw( maxLen ) << vec.x << ", " << std::setw( maxLen ) << vec.y << ")";
	return os;
}

// Easy printing of a FVector3
std::ostream& operator<<( std::ostream& os, const FVector3& vec ) {
	os << "(" << std::setw( 4 ) << vec.x << ", " <<
		std::setw( 4 ) << vec.y << ", " <<
		std::setw( 4 ) << vec.z << ")";
	return os;
}