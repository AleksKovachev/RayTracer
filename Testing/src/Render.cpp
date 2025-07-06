#include "Camera.h"
#include "Colors.h" // Color
#include "DebugObjects.h" // getGround
#include "Mesh.h" // PreparedMesh
#include "Render.h"
#include "RenderSettings.h" // RenderData
#include "Scene.h" // Scene, Mesh
#include "Triangle.h"
#include "utils.h" // writeColorToFile
#include <Vectors.h> // FVector3

#include <filesystem>
#include <fstream>
#include <string>

// TODO: Convert functions to a Render class. Global access to memebrs...
std::vector<PreparedMesh> calculateMeshes( const std::vector<Mesh>& meshes, const ColorMode colorMode ) {
    std::vector<PreparedMesh> outGeometry;
    unsigned counter{};

    for ( const Mesh& mesh : meshes ) {
        outGeometry.emplace_back();
        outGeometry[counter++].PrepMesh( mesh, colorMode );
    }

    return outGeometry;
}

std::ofstream prepareScene( const Scene& scene, const std::string* overrideSaveName ) {
    const int& width{ scene.GetSettings().renderWidth };
    const int& height{ scene.GetSettings().renderHeight };
    const std::string& saveDir{ scene.GetSettings().saveDir };
    const std::string& saveName{
        (overrideSaveName == nullptr ? scene.GetSettings().saveName : *overrideSaveName) };

    std::filesystem::create_directories( saveDir );
    std::ofstream ppmFileStream( saveDir + "/" + saveName + ".ppm", std::ios::binary );
    ppmFileStream << "P6\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << scene.GetSettings().maxColorComp << "\n";

    return ppmFileStream;
}

void render( const Scene& scene, const Camera* overrideCamera, const std::string* overrideSaveName ) {
    const int& width{ scene.GetSettings().renderWidth };
    const int& height{ scene.GetSettings().renderHeight };
    const Camera& camera{ ( overrideCamera == nullptr ? scene.GetCamera() : *overrideCamera ) };
    std::ofstream ppmFileStream = prepareScene( scene, overrideSaveName );

    std::vector<PreparedMesh> meshes{
        calculateMeshes( scene.GetMeshes(), scene.GetSettings().colorMode ) };

    //!? Add ground for debugging
    //std::vector<Triangle> ground{ getGround() };
    //meshes.begin()->second.insert( meshes.end(), ground.begin(), ground.end() );

    for ( int y{}; y < height; ++y ) {
        for ( int x{}; x < width; ++x ) {
            FVector3 ray = camera.GenerateRay( x, y );
            Color pixelColor = camera.GetTriangleIntersection( ray, meshes, scene );
            writeColorToFile( ppmFileStream, pixelColor );
        }
        std::cout << "\rLine: " << y + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();
}

void renderCameraMoveAnimation(
    Scene& scene, const FVector3& initialPos, const FVector3& moveWith, const int frames ) {

    Camera camera{};
    camera.Move( initialPos );
    std::string saveName;

    for ( int i{}; i < frames; ++i ) {
        camera.Move( moveWith );
        saveName = "Move" + std::to_string( i );
        render( scene, &camera, &saveName );
    }
}

void renderRotationAroundObject(
    const Scene& scene, const FVector3& initialPos, const int frames ) {

    Camera camera{};
    int frame{};
    std::string saveName;

    camera.Move( initialPos );
    float distToObject{ initialPos.z };

    for ( int frame{}; frame < frames; ++frame ) {
        camera.RotateAroundPoint( { 0.f, 0.f, distToObject }, { 0.f, frame * 10.f, 0.f } );
        saveName = "Orbit" + std::to_string( frame + 1 );
        render( scene, &camera, &saveName );
    }
}