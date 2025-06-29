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
    Matrix3() {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
    }

    Matrix3( std::initializer_list<std::initializer_list<float>> values ) {
        int i{};
        for ( const auto& row : values ) {
            int j = 0;
            for ( const float& val : row ) {
                m[i][j++] = val;
            }
            ++i;
        }
    }

    Matrix3 operator*( const Matrix3& other ) const {
        Matrix3 result;
        for ( int i{}; i < 3; ++i ) {
            for ( int j{}; j < 3; ++j ) {
                result.m[i][j] = 0.f;
                for ( int k{}; k < 3; ++k ) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    FVector3 operator*( const FVector3& vec ) const {
        return {
            vec.dot( {m[0][0], m[1][0], m[2][0]} ),
            vec.dot( {m[0][1], m[1][1], m[2][1]} ),
            vec.dot( {m[0][2], m[1][2], m[2][2]} )
        };
    }

private:
    float m[3][3];
};

// Parent class to all scene objects describing the position of an object in 3D.
class Obj {
public:
    Obj()
        : m_location{ 0.f, 0.f, 0.f }, m_rotation{ 0.f, 0.f, 0.f } {
    }
    Obj( FVector3 pos, FVector3 rot = { 0.f, 0.f, 0.f } )
        : m_location{ pos }, m_rotation{ rot } {
    }
    Obj( Vector3 pos, FVector3 rot = { 0.f, 0.f, 0.f } )
        : m_location{ FVector3( pos ) }, m_rotation{ FVector3( rot ) } {
    }

    // Combine rotations and update orientation matrix
    void UpdateOrientation() {
        Matrix3 rotX = getXRotMatrix( m_rotation.x );
        Matrix3 rotY = getYRotMatrix( m_rotation.y );
        Matrix3 rotZ = getZRotMatrix( m_rotation.z );
        m_orientation = rotZ * rotY * rotX; // Z-Y-X order
    }

    // Apply rotation to a direction vector (to the location)
    FVector3 ApplyRotation( const FVector3& direction ) const {
        return m_orientation * direction;
    }

    // Returns object's current coordinates
    virtual FVector3 getLocation() const = 0;

    // Set absolute object location
    virtual void move( const FVector3& vec ) = 0;
    virtual void rotate( const FVector3& vec ) {
        m_rotation.x += vec.x; // pitch
        m_rotation.y += vec.y; // yaw
        m_rotation.z += vec.z; // roll
        UpdateOrientation();
    }

    // Get forward vector (negative Z in camera space)
    FVector3 GetForwardVector() const {
        return ApplyRotation( { 0, 0, -1 } );
    }

protected:
    FVector3 m_location;
    FVector3 m_rotation; // Euler angles (pitch, yaw, roll) in radians
    Matrix3 m_orientation;

    Matrix3 getXRotMatrix( const float deg ) {
        const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
        float cosR{ cosf( rad ) };
        float sinR{ sinf( rad ) };
        return { {{1.f, 0.f, 0.f},
                 {0.f, cosR,  sinR},
                 {0.f, sinR, cosR}} };
    }

    Matrix3 getYRotMatrix( const float deg ) {
        const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
        float cosR{ cosf( rad ) };
        float sinR{ sinf( rad ) };
        return { {{cosR, 0.f, -sinR},
                 {0.f, 1.f, 0.f},
                 {sinR, 0.f, cosR}} };
    }

    Matrix3 getZRotMatrix( const float deg ) {
        const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
        float cosR{ cosf( rad ) };
        float sinR{ sinf( rad ) };
        return { {{cosR, sinR, 0.f},
                 {-sinR, cosR, 0.f},
                 {0.f, 0.f, 1.f}} };
    }
};

/* Color class that works with integers.
Initializes with R, G, B integer values or normalized float values. */
struct Color : protected Vector3 {
    int& r;
    int& g;
    int& b;

    Color( int in_r = 0, int in_g = 0, int in_b = 0 )
        : r{ x }, g{ y }, b{ z } {
        r = in_r;
        g = in_g;
        b = in_b;
    }
    Color( float in_r, float in_g, float in_b )
        : r{ x }, g{ y }, b{ z } {
        r = static_cast<int>(round( in_r * 255.f ));
        g = static_cast<int>(round( in_g * 255.f ));
        b = static_cast<int>(round( in_b * 255.f ));
    }

    // Copy assignment
    Color& operator=( const Color& other ) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

// Easy printing of a Color (inline prevents linker error as this is in a header)
inline std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}

#endif // BASES_H