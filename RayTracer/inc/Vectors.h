#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <type_traits>


/* FVector2 structure using floats supporting addition with another FVector2 and
a scalar. Initializes using floats or a Vector2. */
struct FVector2 {
    float x, y;
};


/* FVector3 structure using floats supporting addition with another FVector3 and
+= addition. Initializes using floats or a Vector2 and a float. */
class FVector3 {
public:
    float x, y, z;

    FVector3();

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
    FVector3( const FVector2& vec, float in_z );

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
    float GetLength() const;
    // Dot product with given vector
    float Dot( const FVector3& other ) const;
    // Returns normalized version of this vector
    FVector3 normalize();
    // Normalizes this vector in-place
    void NormalizeInPlace();
private:
    float length;

    // Calculates the length/magnitude of this vector
    void CalculateLength();
};


#endif // VECTORS_H
