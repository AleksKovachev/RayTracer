#include <cmath>
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

struct Circle {
    int x{};
    int y{};
    int r{};
    Color color;
};

std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}

int getInt( const int min, const int max ) {
    std::random_device seed;
    std::mt19937 gen( seed() );
    std::uniform_int_distribution<> genInt( min, max );

    return genInt( gen );
}


int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int shapeNum{ getInt( 5, 15 ) };
    const Color offWhiteBG{ 230, 230, 230 };
    Color pixelColor{ offWhiteBG };

    std::vector<Circle> Circles;
    Circles.reserve( shapeNum );

    for ( int i{}; i < shapeNum; ++i )
        Circles.emplace_back( getInt( 0, width ), getInt( 0, height ), getInt( 20, 300 ), Color{ getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) } );

    std::ofstream ppmFileStream( "Circles.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    for ( int shapeIdx{}; shapeIdx < shapeNum; ++shapeIdx )
        std::cout << Circles[shapeIdx].x << " x " << Circles[shapeIdx].y << ", r=" << Circles[shapeIdx].r << "   " << Circles[shapeIdx].color << std::endl;

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            pixelColor = offWhiteBG;
            for ( int shapeIdx{}; shapeIdx < shapeNum; ++shapeIdx ) {
                if ( std::pow( col - Circles[shapeIdx].x, 2 ) + std::pow( row - Circles[shapeIdx].y, 2 ) <= std::pow( Circles[shapeIdx].r, 2 ) ) {
                    pixelColor = Circles[shapeIdx].color;
                }
            }
            ppmFileStream << pixelColor << "  ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << row + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();

    return 0;
}
