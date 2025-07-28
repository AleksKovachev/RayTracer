#ifndef VECTORS_H
#define VECTORS_H

//#include <iostream>
#include <type_traits> // enable_if_t, is_arithmetic_v


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

    // Default constructor initializes all values to 0.0f.
    FVector3();

    // Constructor that accepts any combination of int/float values
    // @param[in] in_x: The x value of the vector.
    // @param[in] in_y: The y value of the vector.
    // @param[in] in_z: The z value of the vector.
    template <typename T1, typename T2, typename T3,
        typename = std::enable_if_t<
        std::is_arithmetic_v<T1> &&
        std::is_arithmetic_v<T2> &&
        std::is_arithmetic_v<T3>>>
    FVector3( T1 in_x, T2 in_y, T3 in_z )
        : x{ static_cast<float>(in_x) },
        y{ static_cast<float>(in_y) },
        z{ static_cast<float>(in_z) } {
        CalculateLength();
    }
    // Constructor that takes an FVector2 for the x and y values and a z value.
    // @param[in] vec: An FVector2 value that contains the "x" and the "y" values.
    // @param[in] in_z: The "z" value for this vector.
    FVector3( const FVector2&, float );

    // Adds a vector to this one.
    // @param[in] other: The vector that's beeing added.
    // @return The resulting vector.
    FVector3 operator+( const FVector3& ) const;

    // Subtracts a vector from this one.
    // @param[in] other: The vector that's beeing subtracted.
    // @return The resulting vector.
    FVector3 operator-( const FVector3& ) const;

    // Inverses this vector.
    // @return The inverse vector.
    FVector3 operator-() const;

    // In-place addition of two vectors.
    // @param[in] other: The vector that's beeing added to this one.
    // @return This vector, modified to the resulting addition.
    FVector3& operator+=( const FVector3& );

    // Compares two vectors.
    // @param[in] other: The vectors this one is being compared to.
    // @return If the vectors are identical.
    bool operator==( const FVector3& );

    // Scalar multiplication. Multiplies each vector component by the given scalar.
    // @param[in] val: The value each component of this vector is being multiplied by.
    // @return The resulting vector.
    FVector3 operator*( const float& ) const;

    // Cross product.
    // @param[in] other: The second vector that will be in the cross product calculation.
    // @return The resulting vector.
    FVector3 operator*( const FVector3& ) const;

    // ReGets turns vector length/magnitude.
    // @return the length as a float.
    float GetLength();

    // Calculates the dot product with given vector.
    // @param[in] other: The other vector that will be used for the calculation.
    // @return The resulting dot product as a float number.
    float Dot( const FVector3& ) const;

    // Gets normalized version of this vector.
    // @return An exact copy of this vector, but normalized to a unit length.
    FVector3 Normalize() const;

    // In-place normalizes this vector to a unit length.
    void NormalizeInPlace();
private:
    bool lengthCalculated; // a flag that checks if length needs to be recalculated.
    float length;

    // Calculates the length/magnitude of this vector.
    void CalculateLength();
};

#endif // VECTORS_H
