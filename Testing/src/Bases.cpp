#include "Bases.h"

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

void Obj::Rotate( const FVector3& vec ) {
    Matrix3 rotX = GetXRotMatrix( vec.x ); // pitch
    Matrix3 rotY = GetYRotMatrix( vec.y ); // yaw
    Matrix3 rotZ = GetZRotMatrix( vec.z ); // roll
    m_orientation = rotZ * rotY * rotX; // Z-Y-X order
}

Matrix3 Obj::GetXRotMatrix( const float deg ) const {
    const float rad{ static_cast<float>(deg * (std::numbers::pi / 180.f)) };
    float cosR{ cosf( rad ) };
    float sinR{ sinf( rad ) };
    return { {{1.f, 0.f, 0.f},
             {0.f, cosR,  sinR},
             {0.f, sinR, cosR}} };
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

Color::Color( float in_r, float in_g, float in_b )
    : r{ static_cast<int>(round(in_r * 255.f)) },
    g{ static_cast<int>(round( in_g * 255.f)) },
    b{ static_cast<int>(round(in_b * 255.f)) } {
}

Color& Color::operator=( const Color& other ) {
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

// Easy printing of a Color (inline prevents linker error as this is in a header)
inline std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}
