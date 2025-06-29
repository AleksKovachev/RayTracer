#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <type_traits>

// Vector2 structure supporting addition with another Vector2.
struct Vector2 {
    int x, y;

    Vector2 operator+( const Vector2& val ) const;
};

/* FVector2 structure using floats supporting addition with another FVector2 and
a scalar. Initializes using floats or a Vector2. */
struct FVector2 {
    float x, y;

    FVector2( float in_x = 0.f, float in_y = 0.f )
        : x{ in_x }, y{ in_y } {
    }
    FVector2( Vector2 vec )
        : x{ static_cast<float>(vec.x) }, y{ static_cast<float>(vec.y) } {
    }

    FVector2 operator+( const float val ) const;
    FVector2 operator+( const FVector2& val ) const;
};

// Vector3 structure supporting addition with another Vector3 and += addition.
struct Vector3 {
    int x, y, z;

    Vector3 operator+( const Vector3& other ) const;
    Vector3& operator+=( const Vector3& other );
};

/* FVector3 structure using floats supporting addition with another FVector3 and
+= addition. Initializes using floats or a Vector2 and a float. */
class FVector3 {
public:
    float x, y, z;

    FVector3() : x{ 0.f }, y{ 0.f }, z{ 0.f } { init(); };

    // Make c-tor accept any combination of int/float values
    template <typename T1, typename T2, typename T3,
        typename = std::enable_if_t<
        std::is_arithmetic_v<T1> &&
        std::is_arithmetic_v<T2> &&
        std::is_arithmetic_v<T3>>>
    FVector3( T1 in_x, T2 in_y, T3 in_z )
        : x{ static_cast<float>(in_x) },
        y{ static_cast<float>(in_y) },
        z{ static_cast<float>(in_z) } {
        init();
    }
    FVector3( const FVector2& vec, float in_z )
        : x{ vec.x }, y{ vec.y }, z{ in_z } {
        init();
    }
    FVector3( const Vector3& vec )
        : x{ static_cast<float>(vec.x) },
        y{ static_cast<float>(vec.y) },
        z{ static_cast<float>(vec.z) } {
        init();
    }

    void init();

    FVector3 operator+( const FVector3& other ) const;
    FVector3 operator-( const FVector3& other ) const;
    FVector3 operator-() const;
    FVector3& operator+=( const FVector3& other );
    // Scalar multiplication
    FVector3 operator*( const float& other ) const;
    // Cross product
    FVector3 operator*( const FVector3& other ) const;

    // Returns vector length/magnitude
    float getLength() const;
    // Dot product with given vector
    float dot( const FVector3& other ) const;
    // Returns normalized version of this vector
    FVector3 normalize();
    // Normalizes this vector in-place
    void normalizeInPlace();
private:
    float length;

    // Calculates the length/magnitude of this vector
    void calculateLength();
};

// Easy printing of a Vector2
std::ostream& operator<<( std::ostream& os, const Vector2& vec );

// Easy printing of a FVector3
std::ostream& operator<<( std::ostream& os, const FVector3& vec );

#endif // VECTORS_H
