#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Bases.h"
#include "Camera.h"
#include "SceneObjects.h"
#include "SpaceConversions.h"
#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"


struct Params {
    const int width;
    const int height;
    const int maxColorComp;
    const Color BGColor{ 0, 0, 0 };
    const Color shapeColor{ 17, 230, 181 };
    std::string fileName;
    Camera camera;

    int getMaxColorComp( const int colorDepth ) const {
        return static_cast<int>( pow( 2, colorDepth ) - 1 );
    }

    Params( const int width, const int height, const Camera& camera, const std::string& fileName, int colorDepth = 8 )
        : width{ width }, height{ height }, camera{ camera }, fileName{
        fileName
        }, maxColorComp{ getMaxColorComp( colorDepth ) } {
    }
};

void render( const std::vector<Triangle>& triangles, const Params& params ) {
    std::ofstream ppmFileStream( params.fileName + ".ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << params.width << " " << params.height << "\n";
    ppmFileStream << params.maxColorComp << "\n";

    Color pixelColor{};

    FVector3 ray{};

    for ( int y{}; y < params.height; ++y ) {
        for ( int x{}; x < params.width; ++x ) {
            ray = params.camera.generateRay( x, y );
            pixelColor = params.camera.getTriangleIntersection( ray, triangles, params.camera );

            ppmFileStream << pixelColor << "  ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << y + 1 << " / " << params.height << std::flush;
    }
 
    ppmFileStream.close();
}

int main() {
    Camera camera{};
    Params params{ 1920, 1080, camera, "renders/Pyramid" };
    // Lecture Triangle
    Triangle triangle1{ {-1.75, -1.75, -3}, {1.75, -1.75, -3}, {0, 1.75, -3} };
    triangle1.color = Color( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );

    // Made-up Traingle
    Triangle triangle2{ {-2.31, -2.35, -4.3}, {5.03, -0.93, -5.2}, {1.87, 3.17, -3.8} };
    triangle2.color = Color( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );

    // Two Triangles overlapping
    std::vector<Triangle> tris;
    tris.reserve( 2 );
    tris.push_back( triangle1 );
    tris.push_back( triangle2 );

    std::vector<Triangle> pyramid{ getPyramid() };

    render( pyramid, params);

    return 0;
}
