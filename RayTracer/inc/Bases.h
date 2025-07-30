#ifndef BASES_H
#define BASES_H


#include <initializer_list> // initializer_list

#include "Vectors.h" // FVector3


class Matrix3 {
public:
	// Identity matrix by default.
	Matrix3();

	// @param[in] values: An initializer list of all values forming the matrix.
	Matrix3( const std::initializer_list<std::initializer_list<float>>& );

	// @param[in] v1: The first vector to form the first matrix row.
	// @param[in] v2: The second vector to form the second matrix row.
	// @param[in] v3: The third vector to form the third matrix row.
	Matrix3( const FVector3&, const FVector3&, const FVector3& );

	// Get the inverse of this matrix. Return transposed matrix.
	// @return A new Matrix3 object that is the inverse of this one.
	Matrix3 GetInverse() const;

	// Multiplication with another row-major Matrix3.
	// @param[in] other: The other matrix this one will be multiplied with.
	// @return The resulting matrix.
	Matrix3 operator*( const Matrix3& ) const;

	// In-place multiplication with another row-major Matrix3.
	// @param[in] other: The other matrix this one will be multiplied with.
	// @return The same matrix, modified with the multiplication result.
	Matrix3& operator*=( const Matrix3& );

	// Multiplication of a column-major vector with a row-major matrix.
	// @param[in] vec: The column-major vector.
	// @param[in] mat: The row-major matrix.
	// @return The resulting vector.
	friend FVector3 operator*( const FVector3&, const Matrix3& );
private:
	float m[3][3]; // Matrix rows and columns.
};


// Parent class to all scene objects describing the position of an object in 3D.
class Obj {
public:
	Obj();

	// @param[in] pos: The initial position of the object.
	Obj( const FVector3& );

	virtual ~Obj() = default;

	// Apply the object rotation to a direction vector.
	// @param[in] direction: The vector to which the object rotation is applied.
	// @return The same direction vector rotated to the object's matrix.
	FVector3 ApplyRotation( const FVector3& ) const;

	// Returns object's current coordinates.
	// @return The object's current location.
	virtual FVector3 GetLocation() const = 0;

	// Set absolute object location.
	// @param[in] pos: Relative X, Y, Z offset.
	virtual void Move( const FVector3& );

	// Rotate to an absolute angle.
	// @param[in] vec: Absolute X, Y, Z rotation angles.
	virtual void Rotate( const FVector3& );
	// Rotate to an absolute angle.
	// @param[in] x: Абсолуте rotation angles for the X axis.
	// @param[in] y: Абсолуте rotation angles for the Y axis.
	// @param[in] z: Абсолуте rotation angles for the Z axis.
	virtual void Rotate( const float, const float, const float );

	// Get forward vector (negative Z in camera space).
	// @return The forward vector.
	FVector3 GetForwardVector() const;

	// Get the camera's current orientation matrix.
	// @return The orientation matrix of the camera.
	Matrix3 GetOrientationMatrix() const;

	// Set the camera orientation matrix directly.
	// @param[in] matrix: The orientation matrix to set.
	void SetOrientation( const Matrix3& );

	friend class Scene;

protected:
	FVector3 m_position;
	// Each row holds the direction of the rotated unit vector of the camera's local coordinate system.
	Matrix3 m_orientation; // Euler angles (pitch, yaw, roll) in radians.

	// Returns the Rotation Matrix for the X axis based on a given angle.
	// @param[int] deg: The angle in degrees at which the matrix should be generated.
	// @return The resulting matrix.
	Matrix3 GetXRotMatrix( const float ) const;

	// Returns the Rotation Matrix for the Y axis based on a given angle.
	// @param[int] deg: The angle in degrees at which the matrix should be generated.
	// @return The resulting matrix.
	Matrix3 GetYRotMatrix( const float ) const;

	// Returns the Rotation Matrix for the Z axis based on a given angle.
	// @param[int] deg: The angle in degrees at which the matrix should be generated.
	// @return The resulting matrix.
	Matrix3 GetZRotMatrix( const float ) const;
};


#endif // BASES_H