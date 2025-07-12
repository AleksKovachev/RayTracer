#ifndef COLORS_H
#define COLORS_H


/* Color class that works with integers.
Initializes with R, G, B integer values or normalized float values. */
struct Color {
    int r;
    int g;
    int b;

    // @param[in] in_r: The red color value as an integer. Defaults to 0.
    // @param[in] in_g: The green color value as an integer. Defaults to 0.
    // @param[in] in_b: The blue color value as an integer. Defaults to 0.
    Color( const int in_r = 0, const int in_g = 0, const int in_b = 0 );
    // @param[in] in_r: The red color value as a normalized float [0.0, 1.0].
    // @param[in] in_g: The green color value as a normalized float [0.0, 1.0].
    // @param[in] in_b: The blue color value as a normalized float [0.0, 1.0].
    Color( const float, const float, const float );

    // Get the color's max(r, g, b) value.
    // @return An integer value of the max component.
    int GetMaxComponent();

    // Copy assignment.
    // @param[in] The color to copy.
    // @return The copied color.
    Color& operator=( const Color& );

    // Compares two colors.
    // @param[in] other: The color this one is being compared to.
    // @return If the colors are identical.
    bool operator==( const Color& ) const;
    // Divides each color component by a given value.
    // @param[in] val: The value by which each component is divided.
    // @return The resulting color.
    Color operator/( const int ) const;
    // Subtracts a color from this one.
    // @param[in] other: The color that's beeing subtracted.
    // @return The resulting color.
    Color operator-( const Color& ) const;
    // In-place addition of two colors.
    // @param[in] other: The color that's beeing added to this one.
    // @return This color, modified to the resulting addition.
    Color& operator+=( const Color& );
    // In-place subtraction of two colors.
    // @param[in] other: The color that's beeing subtracted from this one.
    // @return This color, modified to the resulting subtraction.
    Color& operator-=( const Color& );
    // In-place multiplication of two colors.
    // @param[in] other: The color that's beeing multiplied by this one.
    // @return This color, modified to the resulting multiplication.
    Color& operator*=( const Color& );
    // In-place division of a color by a value.
    // @param[in] val: The value this color is beeing divided by.
    // @return This color, modified to the resulting division.
    Color& operator/=( const int );
};


enum class ColorMode {
    LoadedMaterial, // Use the color from the mesh' material.
    RandomMeshColor, // Use a random color for each mesh.
    RandomTriangleColor // Use a random color for each triangle in the scene.
};


namespace Colors {
    const Color Black{ 0, 0, 0 };
    const Color White{ 255, 255, 255 };
    const Color Red{ 255, 0, 0 };
    const Color Green{ 0, 255, 0 };
    const Color Blue{ 0, 0, 255 };
    const Color Yellow{ 255, 255, 0 };
    const Color Magenta{ 255, 0, 255 };
    const Color Cyan{ 0, 255, 255 };
    const Color SkyBlue{ 0, 128, 255 };
    const Color Orange{ 255, 128, 0 };
    const Color Lime{ 128, 255, 0 };
    const Color LightMagenta{ 255, 128, 255 }; // Pink
    const Color LightBlue{ 128, 128, 255 };
    const Color LightRed{ 255, 128, 128 };
}

#endif // COLORS_H