#include "Bases.h" // Color
#include "Camera.h"
#include "Render.h"
#include "Scene.h" // Scene, Mesh
#include "DebugObjects.h" // getGround
#include "Triangle.h"
#include "Types.h" // MeshInfo
#include "utils.h" // getRandomColor, writeColorToFile
#include <Vectors.h> // FVector3

#include <filesystem>
#include <fstream>
#include <string>

// TODO: Convert functions to a Render class. Global access to memebrs...
MeshInfo calculateMeshes( const std::vector<Mesh>& meshes, const ColorMode colorMode ) {
    MeshInfo outGeometry;

    for ( const Mesh& mesh : meshes ) {
        outGeometry.insert( { &mesh, std::vector<Triangle>() } );
        const std::vector<FVector3>& vertices = mesh.GetVertices();
        size_t vertSize = vertices.size();
        const std::vector<int>& triangles = mesh.GetTriangles();
        for ( size_t triIdx{}; triIdx + 2 < triangles.size(); triIdx += 3 ) {
            // Skip a triangle if it requires a vertex with an invalid index
            bool idx1ok = triangles[triIdx] > vertSize || triangles[triIdx] < 0;
            bool idx2ok = triangles[triIdx + 1] > vertSize || triangles[triIdx + 1] < 0;
            bool idx3ok = triangles[triIdx + 2] > vertSize || triangles[triIdx + 2] < 0;
            if ( idx1ok || idx2ok || idx3ok )
                continue;

            std::vector<Triangle>& meshTriangles = outGeometry[&mesh];
            meshTriangles.emplace_back(vertices[triangles[triIdx]],
                vertices[triangles[triIdx + 1]],
                vertices[triangles[triIdx + 2]] );

            if ( colorMode == ColorMode::RandomMeshColor )
                meshTriangles[meshTriangles.size() - 1].color = mesh.albedo;
            else if ( colorMode == ColorMode::RandomTriangleColor )
                meshTriangles[meshTriangles.size() - 1].color = getRandomColor();
        }
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

    MeshInfo meshes{ calculateMeshes( scene.GetMeshes(), scene.GetSettings().colorMode ) };

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