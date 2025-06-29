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
    std::ofstream ppmFileStream( params.fileName + ".ppm", std::ios::binary );
    ppmFileStream << "P6\n";
    ppmFileStream << params.width << " " << params.height << "\n";
    ppmFileStream << params.maxColorComp << "\n";

    Color pixelColor{};

    FVector3 ray{};

    for ( int y{}; y < params.height; ++y ) {
        for ( int x{}; x < params.width; ++x ) {
            ray = params.camera.generateRay( x, y );
            pixelColor = params.camera.getTriangleIntersection( ray, triangles, params.camera );
            writeColorToFile( ppmFileStream, pixelColor );
        }
        std::cout << "\rLine: " << y + 1 << " / " << params.height << std::flush;
    }

    ppmFileStream.close();
}


void changeFOV( Camera& camera ) {
    // FoV => camera.z = 30, imgPlane.z = -20 => 50mm
    FVector3 cameraLoc = camera.getLocation();
    cameraLoc.z = 30.f;
    camera.move( cameraLoc );
    FVector3 imgPlaneLoc = camera.m_imgPlane.getLocation();
    imgPlaneLoc.z = -20.f;
    camera.m_imgPlane.move( imgPlaneLoc );
}

void renderOverlappingTriangles( Params& params ) {
    params.fileName = "Triangles";

    // Two Triangles overlapping
    std::vector<Triangle> tris;
    tris.reserve( 2 );

    // Lecture Triangle
    Triangle triangle1{ {-1.75f, -1.75f, -3.f}, {1.75f, -1.75f, -3.f}, {0.f, 1.75f, -3.f} };
    triangle1.color = Color( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );

    // Made-up Traingle
    Triangle triangle2{ {-2.31f, -2.35f, -4.3f}, {5.03f, -0.93f, -5.2f}, {1.87f, 3.17f, -3.8f} };
    triangle2.color = Color( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );

    tris.push_back( triangle1 );
    tris.push_back( triangle2 );

    render( tris, params );
}

void renderPyramid( Params& params ) {
    params.fileName = "Pyramid";

    Pyramid pyramid{ {0, 0, 0 } };
    std::vector<Triangle> ground{getGround()};
    pyramid.shape.insert( pyramid.shape.end(), ground.begin(), ground.end() );

    FVector3 location{ pyramid.getLocation() };
    location.y = 1.f;
    //pyramid.move( location );
    params.camera.moveRel( { 0.f, 0.f, 0.f } );
    params.camera.rotate( { 0.f, 0.f, 0.f } );

    render( pyramid.shape, params );
}

void renderCameraTruckAnimation( std::vector<Triangle> shape, Params& params ) {
    for ( int i{}; i < 30; ++i ) {
        FVector3 camCurrLoc = params.camera.getLocation();
        camCurrLoc.x -= 0.2f;
        params.camera.move( camCurrLoc );
        params.fileName = "Truck" + std::to_string( i );
        render( shape, params );
    }
}

void renderPyramidTurntable( Params& params ) {
    Pyramid pyramid{ {0, 0, 0 } };

    const FVector3 pyramidLocation = pyramid.getLocation();
    const FVector3 distToPyramid{ params.camera.getLocation() - pyramidLocation };

    int frame{};
    do {
        FVector3 camCurrLoc = params.camera.getLocation();
        params.camera.moveRel( distToPyramid );
        params.camera.rotate( { 0.f, 5.f, 0.f } );
        params.camera.moveRel( -distToPyramid );

        params.fileName = "PyramidTurntable" + std::to_string( frame + 1 );
        render( pyramid.shape, params );
        ++frame;
    } while ( frame < 10 );
}


int main() {
    Camera camera{};
    Params params{ 1920, 1080, camera, "Test" };

    //changeFOV( camera );

    //renderOverlappingTriangles( params );

    renderPyramid( params );
    //renderPyramidTurntable( params );

    //renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

    // Camera Rotation

    //params.camera.rotate( { 0.f, 20.f, 0.f } );
    //params.camera.GetForwardVector();
    //render( pyramid.shape, params);


    return 0;
}
