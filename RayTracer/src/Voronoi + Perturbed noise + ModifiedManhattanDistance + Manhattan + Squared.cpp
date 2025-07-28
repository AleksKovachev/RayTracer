#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
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

// Get fast sudo-random values per coordinate
int hash( int x, int y ) {
    return (x * 73856093) ^ (y * 19349663);
}

// Create the noise and normalize the values
float noise( int x, int y ) {
    return (hash( x, y ) & 0xFFFF) / 65535.0f; // Normalize to [0, 1]
}

int manhattanDistance( const Vector2& a, const Vector2& b ) {
    return std::abs( a.x - b.x ) + std::abs( a.y - b.y );
}

int madeUpDistance( const Vector2& a, const Vector2& b ) {
    return std::abs( a.x - b.x ) * std::abs( a.y - b.y );
}

int madeUpDistance2( const Vector2& a, const Vector2& b ) {
    return std::abs( a.x - b.x ) ^ std::abs( a.y - b.y );
}


int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int numSites{ 60 };

    const Color offWhiteBG{ 230, 230, 230 };
    Color pixelColor{ offWhiteBG };
    std::cout << std::setfill( '_' );

    std::vector<Vector2> sites;
    std::vector<Color> siteColors;
    sites.reserve( numSites );
    siteColors.reserve( numSites );

    for ( int i = 0; i < numSites; ++i ) {
        sites.emplace_back( getInt( 0, width - 1 ), getInt( 0, height - 1 ) );
        siteColors.emplace_back( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );
    }

    std::ofstream ppmFileStream( "Perturbed.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            int minDist = std::numeric_limits<int>::max();
            int closest = 0;

            for ( int i{}; i < numSites; ++i ) {
                // Slightly offset pixel - pertrub pixel coordinates
                float strength = 10.0f; // tweak this for more or less distortion
                float nCol = col + (noise( col, row ) - 0.5f) * strength;
                float nRow = row + (noise( row, col ) - 0.5f) * strength;

                // Feed to whatever distance function
                int dist = madeUpDistance( { static_cast<int>( nCol ), static_cast<int>( nRow ) }, sites[i] );

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
