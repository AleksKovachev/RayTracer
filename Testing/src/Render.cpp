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


Render::Render( const Scene& scene )
    : m_scene{ scene },
    m_overrideCamera{ nullptr },
    m_overrideSaveName{ nullptr },
    m_frames { 1 } {
}

Render::Render( const Scene& scene, Camera& overrideCamera )
    : m_scene{ scene },
    m_overrideCamera{ &overrideCamera },
    m_overrideSaveName{ nullptr },
    m_frames{ 1 } {
}

Render::Render( const Scene& scene, Camera& overrideCamera, const std::string& overrideSaveName )
    : m_scene{ scene },
    m_overrideCamera{ &overrideCamera },
    m_overrideSaveName{ &overrideSaveName },
    m_frames{ 1 } {
}

Render::~Render() {
    delete m_overrideCamera;
    delete m_overrideSaveName;
}


void Render::RenderImage() {
    const int& width{ m_scene.GetSettings().renderWidth };
    const int& height{ m_scene.GetSettings().renderHeight };
    const Camera& camera{ (m_overrideCamera == nullptr ? m_scene.GetCamera() : *m_overrideCamera) };
    std::ofstream ppmFileStream = PrepareScene();

    for ( int y{}; y < height; ++y ) {
        for ( int x{}; x < width; ++x ) {
            Ray ray = camera.GenerateRay( x, y );
            Color pixelColor = camera.GetTriangleIntersection(
                ray, m_scene, m_scene.GetReflectionDepth() );
            writeColorToFile( ppmFileStream, pixelColor );
        }
        std::cout << "\rLine: " << y + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();
}

std::ofstream Render::PrepareScene() {
    const Settings& settings{ m_scene.GetSettings() };

    const int& width{ settings.renderWidth };
    const int& height{ settings.renderHeight };
    const std::string& saveDir{ settings.saveDir };
    const std::string& saveName{
        (m_overrideSaveName == nullptr ? settings.saveName : *m_overrideSaveName) };

    std::filesystem::create_directories( saveDir );
    std::ofstream ppmFileStream( saveDir + "/" + saveName + ".ppm", std::ios::binary );
    ppmFileStream << "P6\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << settings.maxColorComp << "\n";

    return ppmFileStream;
}

void Render::RenderCameraMoveAnimation(
   const FVector3& initialPos, const FVector3& moveWith ) {

    Camera camera{};
    m_overrideCamera = &camera;
    camera.Move( initialPos );
    std::string saveName;

    for ( int frame{}; frame < m_frames; ++frame ) {
        camera.Move( moveWith );
        saveName = "Move" + std::to_string( frame );
        RenderImage();
    }
}

void Render::RenderRotationAroundObject( const FVector3& initialPos ) {

    Camera camera{};
    m_overrideCamera = &camera;
    std::string saveName;

    camera.Move( initialPos );
    float distToObject{ initialPos.z };

    for ( int frame{}; frame < m_frames; ++frame ) {
        camera.RotateAroundPoint( { 0.f, 0.f, distToObject }, { 0.f, frame * 10.f, 0.f } );
        saveName = "Orbit" + std::to_string( frame + 1 );
        RenderImage();
    }
}