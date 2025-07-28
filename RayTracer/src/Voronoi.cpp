#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>


struct Color {
    int r{ 0 };
    int g{ 0 };
    int b{ 0 };
};

struct Vector2 {
    int x, y;
};


std::ostream& operator<<( std::ostream& os, const Vector2& vec ) {
    os << "(" << std::setw( 4 ) << vec.x << ", " << std::setw( 4 ) << vec.y << ")";
    return os;
}

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

int squaredDistance( const Vector2& a, const Vector2& b ) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}


int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int numSites{ 20 };

    const Color offWhiteBG{ 230, 230, 230 };

    std::vector<Vector2> sites;
    std::vector<Color> siteColors;
    sites.reserve( numSites );
    siteColors.reserve( numSites );

    for ( int i = 0; i < numSites; ++i ) {
        sites.emplace_back( getInt( 0, width - 1 ), getInt( 0, height - 1 ) );
        siteColors.emplace_back( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );
    }

    std::ofstream ppmFileStream( "Voronoi.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            int minDist = std::numeric_limits<int>::max();
            int closest = 0;

            for ( int i{}; i < numSites; ++i ) {
                int dist = squaredDistance( { col, row }, sites[i] );
                if ( dist < minDist ) {
                    minDist = dist;
                    closest = i;
                }
            }
            ppmFileStream << siteColors[closest] << " ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << row + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();

    return 0;
}
