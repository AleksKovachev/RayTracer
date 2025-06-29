#include "Vectors.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>

Vector2 Vector2::operator+( const Vector2& val ) const {
	return Vector2( x + val.x, y + val.y );
}

FVector2 FVector2::operator+( const float val ) const {
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
	calculateLength();
}

FVector3 FVector3::operator+( const FVector3& other ) const {
	return { x + other.x, y + other.y, z + other.z };
}

FVector3 FVector3::operator-( const FVector3& other ) const {
	return { x - other.x, y - other.y, z - other.z };
}

FVector3 FVector3::operator-() const {
	return *this * -1;
}

FVector3& FVector3::operator+=( const FVector3& other ) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// Cross product
FVector3 FVector3::operator*( const FVector3& other ) const {
	return { y * other.z - z * other.y,
			 z * other.x - x * other.z,
			 x * other.y - y * other.x };
}

// Scalar multiplication
FVector3 FVector3::operator*( const float& other ) const {
	return { x * other, y * other, z * other };
}


float FVector3::getLength() const {
	return length;
}

float FVector3::dot( const FVector3& other ) const {
	return (x * other.x) + (y * other.y) + (z * other.z);
}

FVector3 FVector3::normalize() {
	if ( length > 0.f )
		return { x / length, y / length, z / length };
	return { 0.f, 0.f, 0.f };
}

void FVector3::normalizeInPlace() {
	if ( length > 0.f ) {
		x /= length;
		y /= length;
		z /= length;
		return;
	}
	x = y = z = 0.f;
}

void FVector3::calculateLength() {
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