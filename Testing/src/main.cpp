#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include "sstream"
#include <cmath>
#include <cstdint>
#include <stack>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <random>
#include <limits>
#include <filesystem>

#include "Bases.h"
#include "Camera.h"
#include "Triangle.h"
#include "SceneObjects.h"
#include "SpaceConversions.h"
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

    Params( const int width, const int height, const Camera& camera, const std::string& fileName, int colorDepth = 8 )
        : width{ width }, height{ height }, camera{ camera }, fileName{
        fileName
        }, maxColorComp{ (1 << colorDepth) - 1 } {
    }
};

void writeColorToFile( std::ofstream& stream, const Color& pixelColor ) {
    unsigned char r = static_cast<unsigned char>(pixelColor.r);
    unsigned char g = static_cast<unsigned char>(pixelColor.g);
    unsigned char b = static_cast<unsigned char>(pixelColor.b);
    stream.write( reinterpret_cast<const char*>(&r), 1 );
    stream.write( reinterpret_cast<const char*>(&g), 1 );
    stream.write( reinterpret_cast<const char*>(&b), 1 );
}

void render( const std::vector<Triangle>& triangles, const Params& params ) {
    std::string saveDir{ "renders" };
    std::filesystem::create_directories( saveDir );
    std::ofstream ppmFileStream( saveDir + "/" + params.fileName + ".ppm", std::ios::binary);
    ppmFileStream << "P6\n";
    ppmFileStream << params.width << " " << params.height << "\n";
    ppmFileStream << params.maxColorComp << "\n";

    Color pixelColor{};

    FVector3 ray{};

    for ( int y{}; y < params.height; ++y ) {
        for ( int x{}; x < params.width; ++x ) {
            ray = params.camera.GenerateRay( x, y );
            pixelColor = params.camera.GetTriangleIntersection( ray, triangles, params.camera );
            writeColorToFile( ppmFileStream, pixelColor );
        }
        std::cout << "\rLine: " << y + 1 << " / " << params.height << std::flush;
    }

    ppmFileStream.close();
}

void renderOverlappingTriangles( Params& params ) {
    params.fileName = "Triangles";

    // Two Triangles overlapping
    std::vector<Triangle> tris;
    tris.reserve( 2 );

    // Lecture Triangle
    Triangle triangle1{ {-1.75f, -1.75f, -3.f}, {1.75f, -1.75f, -3.f}, {0.f, 1.75f, -3.f} };
    triangle1.color = Color( 255, 0, 0 );

    // Made-up Traingle
    Triangle triangle2{ {-2.31f, -2.35f, -4.3f}, {5.03f, -0.93f, -5.2f}, {1.87f, 3.17f, -3.8f} };
    triangle2.color = Color( 0, 255, 0 );

    tris.push_back( triangle1 );
    tris.push_back( triangle2 );

    render( tris, params );
}

void renderCameraTruckAnimation( std::vector<Triangle> shape, Params& params ) {
    for ( int i{}; i < 30; ++i ) {
        FVector3 camCurrLoc = params.camera.GetLocation();
        camCurrLoc.x -= 0.2f;
        params.camera.Move( camCurrLoc );
        params.fileName = "Truck" + std::to_string( i );
        render( shape, params );
    }
}

void renderPyramid( Params& params ) {
    params.fileName = "Pyramid";

    Pyramid pyramid{ { 0, 0, 0 } };
    std::vector<Triangle> ground{getGround()};
    //pyramid.shape.insert( pyramid.shape.end(), ground.begin(), ground.end() );
    params.camera.m_imgPlane.distanceFromCamera = 10.f; // Change FoV

    FVector3 location{ pyramid.GetLocation() };
    location.y = 1.f;
    //pyramid.Move( location );
    params.camera.Move( { 0.f, 100.f, 0.f } );
    params.camera.Rotate( { 90.f, 0.f, 0.f } );

    render( pyramid.shape, params );
}

void renderRotationAroundPyramid( Params& params ) {
    Pyramid pyramid{ {0.f, 0.f, 0.f } };

    int frame{};

    float cameraDist{ 5.f };
    params.camera.Move( { 0.f, 2.f, cameraDist } ); // Set initial position

    do {
        params.camera.Move( { 0.f, 0.f, -cameraDist } );
        params.camera.Rotate( { 0.f, frame * 10.f, 0.f } );
        params.camera.Move( { 0.f, 0.f, cameraDist } );

        params.fileName = "PyramidTurntable" + std::to_string( frame + 1 );
        render( pyramid.shape, params );
        ++frame;
    } while ( frame < 30 );
}

void interactiveRender(Params& params) {
    iniData interactiveData;

    while ( true ) {
        interactiveData = readConfig();

        renderPyramid( params );
    }
}


int main() {
    Camera camera{};
    Params params{ 1920, 1080, camera, "Test" };

    //renderOverlappingTriangles( params );

    //renderPyramid( params );
    //renderRotationAroundPyramid( params );

    //renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

    //interactiveRender( params );

    return 0;
}
