#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <utility>


struct Color {
    int r{};
    int g{};
    int b{};
};

std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}

int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int squareSide{ 250 };
    const std::pair<int, int> anchorPoint{ width / 2, height / 2 };
    const Color offWhite{ 230, 230, 230 };
    const Color skyBlue{ 0, 128, 255 };

    std::ofstream ppmFileStream( "Square.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            if ( (row >= anchorPoint.second - squareSide && row < anchorPoint.second + squareSide)
                && (col >= anchorPoint.first - squareSide && col < anchorPoint.first + squareSide) )
                ppmFileStream << skyBlue << "  ";
            else
                ppmFileStream << offWhite << "  ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << row + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();

    return 0;
}
