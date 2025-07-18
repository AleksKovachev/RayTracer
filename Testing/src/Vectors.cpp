#include "Vectors.h"

#include <cmath> // sqrt


FVector3::FVector3() : x{ 0.f }, y{ 0.f }, z{ 0.f } {
	CalculateLength();
};

FVector3::FVector3( const FVector2& vec, float in_z )
	: x{ vec.x }, y{ vec.y }, z{ in_z } {
	CalculateLength();
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
	lengthCalculated = false;
	return *this;
}

bool FVector3::operator==( const FVector3& other ) {
	return x == other.x && y == other.y && z == other.z;
}

// Cross product
FVector3 FVector3::operator*( const FVector3& other ) const {
	return { y * other.z - z * other.y,
			 z * other.x - x * other.z,
			 x * other.y - y * other.x };
}

// Scalar multiplication
FVector3 FVector3::operator*( const float& val ) const {
	return { x * val, y * val, z * val };
}

float FVector3::GetLength() {
	if ( !lengthCalculated ) {
		CalculateLength();
		lengthCalculated = true;
	}
	return length;
}

float FVector3::Dot( const FVector3& other ) const {
	return (x * other.x) + (y * other.y) + (z * other.z);
}

FVector3 FVector3::Normalize() const {
	if ( length > 0.f )
		return { x / length, y / length, z / length };
	return { 0.f, 0.f, 0.f };
}

void FVector3::NormalizeInPlace() {
	if ( !lengthCalculated ) {
		CalculateLength();
		lengthCalculated = true;
	}

	if ( length > 0.f ) {
		x /= length;
		y /= length;
		z /= length;
		return;
	}
	x = y = z = 0.f;
}

void FVector3::CalculateLength() {
	length = std::sqrt( x * x + y * y + z * z );
}
