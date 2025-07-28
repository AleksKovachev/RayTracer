#ifndef COLORS_H
#define COLORS_H


/* Color class that works with integers.
Initializes with R, G, B integer values or normalized float values. */
struct Color {
    int r;
    int g;
    int b;

    Color( int in_r = 0, int in_g = 0, int in_b = 0 );
    Color( float in_r, float in_g, float in_b );

    int GetMaxComponent();

    // Copy assignment
    Color& operator=( const Color& other );

    bool operator==( const Color& other );
    Color operator/( const int val );
    Color operator-( const Color& other );
    Color& operator+=( const Color& other );
    Color& operator-=( const Color& other );
    Color& operator*=( const Color& other );
    Color& operator/=( const int val );
};


enum class ColorMode {
    LoadedMaterial,
    RandomMeshColor,
    RandomTriangleColor
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