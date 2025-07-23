#ifndef COLORS_H
#define COLORS_H


/* Color class that works with normalized float values.
Initializes with R, G, B integer values or normalized float values.
Initialization with float values does NOT check range as it works with HDR values! */
struct Color {
    float r;
    float g;
    float b;

    // @param[in] in_r: The red color value as a normalized float [0.0, 1.0]. Defaults to 0.
    // @param[in] in_g: The green color value as a normalized float [0.0, 1.0]. Defaults to 0.
    // @param[in] in_b: The blue color value as a normalized float [0.0, 1.0]. Defaults to 0.
    // @param[in] depth: The color bit-depth value. Defaults to 8-bit color
    Color( const float = 0.f, const float = 0.f, const float = 0.f, const int depth = 8 );

    // @param[in] in_r: The red color value as an integer.
    // @param[in] in_g: The green color value as an integer.
    // @param[in] in_b: The blue color value as an integer.
    // @param[in] depth: The color bit-depth value. Defaults to 8-bit color.
    Color( const int in_r, const int in_g, const int in_b, const int depth = 8 );

    // Get the color's max(r, g, b) value.
    // @return A float value of the max component.
    float GetCurrMaxComponent();

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
    Color operator/( const float ) const;

    // In-place division of a color by a value.
    // @param[in] val: The value this color is beeing divided by.
    // @return This color, modified to the resulting division.
    Color& operator/=( const float );

    // Multiplies each color component by a given value.
    // @param[in] val: The value by which each component is multiplied.
    // @return The resulting color.
    Color operator*( const float ) const;

    // In-place multiplication of two colors.
    // @param[in] other: The color that's beeing multiplied by this one.
    // @return This color, modified to the resulting multiplication.
    Color& operator*=( const Color& );

    // Subtracts a color from this one.
    // @param[in] other: The color that's beeing subtracted.
    // @return The resulting color.
    Color operator-( const Color& ) const;

    // In-place subtraction of two colors.
    // @param[in] other: The color that's beeing subtracted from this one.
    // @return This color, modified to the resulting subtraction.
    Color& operator-=( const Color& );

    // Adds a color to this one.
    // @param[in] other: The color that's beeing added.
    // @return The resulting color.
    Color operator+( const Color& ) const;

    // In-place addition of two colors.
    // @param[in] other: The color that's beeing added to this one.
    // @return This color, modified to the resulting addition.
    Color& operator+=( const Color& );
};


namespace Colors {
    const Color Black{ 0.f, 0.f, 0.f };
    const Color White{ 1.f, 1.f, 1.f };
    const Color Red{ 1.f, 0.f, 0.f };
    const Color Green{ 0.f, 1.f, 0.f };
    const Color Blue{ 0.f, 0.f, 1.f };
    const Color Yellow{ 1.f, 1.f, 0.f };
    const Color Magenta{ 1.f, 0.f, 1.f };
    const Color Cyan{ 0.f, 1.f, 1.f };
    const Color SkyBlue{ 0.f, 0.5f, 1.f };
    const Color Orange{ 1.f, 0.5f, 0.f };
    const Color Lime{ 0.5f, 1.f, 0.f };
    const Color LightMagenta{ 1.f, 0.5f, 1.f }; // Pink
    const Color LightBlue{ 0.5f, 0.5f, 1.f };
    const Color LightRed{ 1.f, 0.5f, 0.5f };
}

#endif // COLORS_H