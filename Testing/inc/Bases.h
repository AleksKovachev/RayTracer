#ifndef BASES_H
#define BASES_H

#include <cmath>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <numbers>

#include "Vectors.h"


class Matrix3 {
public:
    // Identity matrix by default
    Matrix3();
    Matrix3( std::initializer_list<std::initializer_list<float>> values );

    Matrix3 operator*( const Matrix3& other ) const;
    friend FVector3 operator*( const FVector3&, const Matrix3& );
private:
    float m[3][3];
};


// Parent class to all scene objects describing the position of an object in 3D.
class Obj {
public:
    Obj()
        : m_position{ 0.f, 0.f, 0.f }, m_rotation{ 0.f, 0.f, 0.f } {
    }
    Obj( FVector3 pos, FVector3 rot = { 0.f, 0.f, 0.f } )
        : m_position{ pos }, m_rotation{ rot } {
    }
    Obj( Vector3 pos, FVector3 rot = { 0.f, 0.f, 0.f } )
        : m_position{ FVector3( pos ) }, m_rotation{ FVector3( rot ) } {
    }

    // Combine rotations and update orientation matrix
    void UpdateOrientation();

    // Apply rotation to a direction vector (to the location)
    FVector3 ApplyRotation( const FVector3& direction ) const {
        return direction * m_orientation;
    }

    // Returns object's current coordinates
    virtual FVector3 getLocation() const = 0;

    // Set absolute object location
    virtual void move( const FVector3& vec ) = 0;
    virtual void rotate( const FVector3& vec );

    // Get forward vector (negative Z in camera space)
    FVector3 GetForwardVector() const {
        return ApplyRotation( { 0, 0, -1 } );
    }

protected:
    FVector3 m_position;
    FVector3 m_rotation; // Euler angles (pitch, yaw, roll) in radians
    Matrix3 m_orientation;

    Matrix3 getXRotMatrix( const float deg ) const;
    Matrix3 getYRotMatrix( const float deg ) const;
    Matrix3 getZRotMatrix( const float deg ) const;
};

/* Color class that works with integers.
Initializes with R, G, B integer values or normalized float values. */
struct Color : protected Vector3 {
    int& r;
    int& g;
    int& b;

    Color( int in_r = 0, int in_g = 0, int in_b = 0 );
    Color( float in_r, float in_g, float in_b );

    // Copy assignment
    Color& operator=( const Color& other );
};

#endif // BASES_H