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

struct Circle {
    int x;
    int y;
    int r;
    Color color;
};

struct Vector2 {
    int x, y;
};

struct Triangle {
    Vector2 A;
    Vector2 B;
    Vector2 C;
    Color color;
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

// Edge function: returns >0 if point `p` is on the left side of edge a→b
int edgeFunction( const Vector2& a, const Vector2& b, const Vector2& p ) {
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}


int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    const int circleCount{ getInt( 5, 25 ) };
    const int triangleCount{ getInt( 10, 30 ) };
    const Color offWhiteBG{ 230, 230, 230 };
    Color pixelColor{ offWhiteBG };
    std::cout << std::setfill( '_' );

    std::vector<Triangle> Triangles;
    Triangles.reserve( triangleCount );

    std::vector<Circle> Circles;
    Circles.reserve( circleCount );

    // Random Triangles
    //for ( int i{}; i < triangleCount; ++i )
    //    Triangles.emplace_back( Vector2( getInt( 0, width ), getInt( 0, height ) ), Vector2( getInt( 0, width ), getInt( 0, height ) ), Vector2( getInt( 0, width ), getInt( 0, height ) ), Color{ getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) } );

    // Random Triangles with a relatively small size
    for ( int i{}; i < triangleCount; ++i ) {
        Vector2 a{ getInt( 0, width ), getInt( 0, height ) };
        Vector2 b{ std::clamp( a.x + getInt( -450, 450 ), 0, width ), std::clamp( a.y + getInt( -350, 350 ), 0, height ) };
        Vector2 c{ std::clamp( b.x + getInt( -450, 450 ), 0, width ), std::clamp( b.y + getInt( -350, 350 ), 0, height ) };
        Triangles.emplace_back( a, b, c, Color{ getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) } );
    }

    for ( int i{}; i < circleCount; ++i )
        Circles.emplace_back( getInt( 0, width ), getInt( 0, height ), getInt( 20, 300 ), Color{ getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) } );

    std::ofstream ppmFileStream( "Shapes.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    std::cout << "Triangles:" << std::endl;
    for ( int triangleIdx{}; triangleIdx < triangleCount; ++triangleIdx )
        std::cout << Triangles[triangleIdx].A << " x " << Triangles[triangleIdx].B << " x " << Triangles[triangleIdx].C << "   (" << Triangles[triangleIdx].color << ")" << std::endl;

    std::cout << "\nCircles:" << std::endl;
    for ( int circleIdx{}; circleIdx < circleCount; ++circleIdx )
        std::cout << std::setw( 4 ) << Circles[circleIdx].x << " x " << std::setw( 4 ) << Circles[circleIdx].y << ", r=" << std::setw( 3 ) << Circles[circleIdx].r << "   (" << Circles[circleIdx].color << ")" << std::endl;

    for ( int row{}; row < height; ++row ) {
        for ( int col{}; col < width; ++col ) {
            pixelColor = offWhiteBG;
            for ( int triangleIdx{}; triangleIdx < triangleCount; ++triangleIdx ) {
                Vector2 Pos{ col, row };
                int w0 = edgeFunction( Triangles[triangleIdx].B, Triangles[triangleIdx].C, Pos );
                int w1 = edgeFunction( Triangles[triangleIdx].C, Triangles[triangleIdx].A, Pos );
                int w2 = edgeFunction( Triangles[triangleIdx].A, Triangles[triangleIdx].B, Pos );
                if ( (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0) )
                    pixelColor = Triangles[triangleIdx].color;
            }
            for ( int circleIdx{}; circleIdx < circleCount; ++circleIdx ) {
                if ( std::pow( col - Circles[circleIdx].x, 2 ) + std::pow( row - Circles[circleIdx].y, 2 ) <= std::pow( Circles[circleIdx].r, 2 ) ) {
                    pixelColor = Circles[circleIdx].color;
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
