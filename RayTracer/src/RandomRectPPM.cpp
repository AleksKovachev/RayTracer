#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>


struct Color {
    int r{};
    int g{};
    int b{};
};

int getInt( const int min, const int max ) {
    std::random_device seed;
    std::mt19937 gen( seed() );
    std::uniform_int_distribution<> genInt( min, max );

    return genInt( gen );
}

// Put in stream with noise
std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << ((color.r) ? std::clamp( color.r + getInt( -100, 100 ), 0, 255 ) : color.r) << " ";
    os << std::setw( 3 ) << ((color.g) ? std::clamp( color.g + getInt( -100, 100 ), 0, 255 ) : color.g) << " ";
    os << std::setw( 3 ) << ((color.b) ? std::clamp( color.b + getInt( -100, 100 ), 0, 255 ) : color.b);
    return os;
}

int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int numRect{ 5 };
    const int colorSwitchHorIdx{ width / numRect };
    const int colorSwitchVerIdx{ height / numRect };
    int colorIdx{ 999 };
    int lastIdx{ 999 };
    std::vector<Color> rectColors( 5 );

    const std::vector<Color> Colors{
        { 255 }, // Red
        { 0, 255 }, // Green
        { 0, 0, 255 }, //Blue
        { 255, 255 }, // Yellow
        { 0, 255, 255 }, // Cyan
        { 255, 0, 255 }, // Magenta
        { 255, 128 }, // Orange
        { 0, 128, 255 }, // Sky Blue
        { 128, 256 }, // Lime
        { 255, 0, 128 } // Pink
    };

    std::ofstream ppmFileStream( "randomRect.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            if ( (col == 0 || col % colorSwitchHorIdx == 0) && (row == 0 || row % colorSwitchVerIdx == 0) ) {
                while ( colorIdx == lastIdx )
                    colorIdx = getInt( 0, 9 );
                lastIdx = colorIdx;
                rectColors[col / colorSwitchHorIdx] = Colors[colorIdx];
            }
            ppmFileStream << rectColors[col / colorSwitchHorIdx] << "  ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << row + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();

    return 0;
}
