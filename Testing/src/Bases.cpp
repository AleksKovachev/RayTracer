#include "Bases.h"
#include "utils.h" // areEqual

#include <cmath> // sinf, cosf
#include <numbers> // pi

// Multiplication with a Matrix3, assuming this is a column-major vector
FVector3 operator*( const FVector3& vec, const Matrix3& mat ) {
    return {
        vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0],
        vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1],
        vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2]
    };
}

Matrix3::Matrix3() {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}

Matrix3::Matrix3( std::initializer_list<std::initializer_list<float>> values ) {
    int i{};
    for ( const auto& row : values ) {
        int j = 0;
        for ( const float& val : row ) {
            m[i][j++] = val;
        }
        ++i;
    }
}

Matrix3::Matrix3( const FVector3& v1, const FVector3& v2, const FVector3& v3 ) {
    m[0][0] = v1.x; m[0][1] = v1.y; m[0][2] = v1.z;
    m[1][0] = v2.x; m[1][1] = v2.y; m[1][2] = v2.z;
    m[2][0] = v3.x; m[2][1] = v3.y; m[2][2] = v3.z;
}

Matrix3 Matrix3::operator*( const Matrix3& other ) const {
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

Matrix3& Matrix3::operator*=( const Matrix3& other ) {
    *this = *this * other;
    return *this;
}

Obj::Obj() : m_position{ 0.f, 0.f, 0.f }, m_rotation{ 0.f, 0.f, 0.f } {}

Obj::Obj( FVector3 pos ) : m_position{ pos }, m_rotation{ 0.f, 0.f, 0.f } {}

FVector3 Obj::ApplyRotation( const FVector3& direction ) const {
    return direction * m_orientation;
}

void Obj::Move( const FVector3& pos ) {
    const FVector3 moveDirInWorldSpace{ pos * m_orientation };
    m_position += moveDirInWorldSpace;
}

void Obj::Rotate( const FVector3& vec ) {
    bool assigned{ false };

    if ( !areEqual( vec.x, 0.f ) ) { // pitch
        m_orientation = GetXRotMatrix( vec.x );
        assigned = true;
    }
    if ( !areEqual( vec.y, 0.f ) ) { // yaw
        if ( assigned )
            m_orientation *= GetYRotMatrix( vec.y );
        else {
            m_orientation = GetYRotMatrix( vec.y );
            assigned = true;
        }
    }
    if ( !areEqual( vec.z, 0.f ) ) { // roll
        if ( assigned )
            m_orientation *= GetZRotMatrix( vec.z );
        else
            m_orientation = GetZRotMatrix( vec.z );
    }
}

void Obj::Rotate( const float x, const float y, const float z ) {
    bool assigned{ false };

    if ( !areEqual( x, 0.f ) ) { // pitch
        m_orientation = GetXRotMatrix( x );
        assigned = true;
    }
    if ( !areEqual( y, 0.f ) ) { // yaw
        if ( assigned )
            m_orientation *= GetYRotMatrix( y );
        else {
            m_orientation = GetYRotMatrix( y );
            assigned = true;
        }
    }
    if ( !areEqual( z, 0.f ) ) { // roll
        if ( assigned )
            m_orientation *= GetZRotMatrix( z );
        else
            m_orientation = GetZRotMatrix( z );
    }
}

FVector3 Obj::GetForwardVector() const {
    return ApplyRotation( { 0, 0, -1 } );
}

Matrix3 Obj::GetXRotMatrix( const float deg ) const {
    const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
    float cosR{ cosf( rad ) };
    float sinR{ sinf( rad ) };
    return { {{1.f, 0.f, 0.f},
             {0.f, cosR, sinR},
             {0.f, -sinR, cosR}} };
}

Matrix3 Obj::GetYRotMatrix( const float deg ) const {
    const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
    float cosR{ cosf( rad ) };
    float sinR{ sinf( rad ) };
    return { {{cosR, 0.f, -sinR},
             {0.f, 1.f, 0.f},
             {sinR, 0.f, cosR}} };
}

Matrix3 Obj::GetZRotMatrix( const float deg ) const {
    const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
    float cosR{ cosf( rad ) };
    float sinR{ sinf( rad ) };
    return { {{cosR, sinR, 0.f},
             {-sinR, cosR, 0.f},
             {0.f, 0.f, 1.f}} };
}
