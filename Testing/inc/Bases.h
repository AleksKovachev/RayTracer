#ifndef BASES_H
#define BASES_H


#include <initializer_list> // initializer_list

#include "Vectors.h" // FVector3


class Matrix3 {
public:
    // Identity matrix by default
    Matrix3();
    Matrix3( std::initializer_list<std::initializer_list<float>> );
    Matrix3( const FVector3&, const FVector3&, const FVector3& );

    Matrix3 operator*( const Matrix3& ) const;
    friend FVector3 operator*( const FVector3&, const Matrix3& );
    Matrix3& operator*=( const Matrix3& );
private:
    float m[3][3];
};


// Parent class to all scene objects describing the position of an object in 3D.
class Obj {
public:
    Obj();
    Obj( FVector3 pos );

    virtual ~Obj() = default;

    // Apply rotation to a direction vector (to the location)
    FVector3 ApplyRotation( const FVector3& direction ) const;

    // Returns object's current coordinates
    virtual FVector3 GetLocation() const = 0;

    // Set absolute object location
    virtual void Move( const FVector3& pos );

    // Rotate to an absolute angle
    virtual void Rotate( const FVector3& vec );
    // Rotate to an absolute angle
    virtual void Rotate( const float, const float, const float );

    // Get forward vector (negative Z in camera space)
    FVector3 GetForwardVector() const;

    friend class Scene;

protected:
    FVector3 m_position;
    FVector3 m_rotation; // Euler angles (pitch, yaw, roll) in radians
    // Each row holds the direction of the rotated unit vector of the camera's local coordinate system
    Matrix3 m_orientation;

    Matrix3 GetXRotMatrix( const float deg ) const;
    Matrix3 GetYRotMatrix( const float deg ) const;
    Matrix3 GetZRotMatrix( const float deg ) const;
};


#endif // BASES_H