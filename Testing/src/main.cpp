#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
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

#include "Camera.h"
#include "Render.h"
#include "Scene.h"
#include "Vectors.h"

// TODO: Finish the interactive rendering.
// TODO: Finish the OBJ file parser.
// TODO: Make the Triangle class hold indices to the vertices, not their copies.
// TODO: Implement reading and applying Normal maps.
// TODO: Implement GI and antialiasing.
// TODO: Implement BVH Tree and SAH.
// TODO: Implement bitmap texture mapping without stretching. 2 types - Fit, which
//       puts blakc bars around the texture to fit the geometry, and Fill, which
//       places the texture making it go "out of bounds" but fill the geometry.


/*
 * The whole Render Engine is built for ease of use, which costs a lot of memory.
 * To make it more efficient for larger scenes, the PreparedMesh class must be
 * removed entirely. The program should only work with the Mesh class, which should
 * also store a vector<Material>. Then, the IntersectionData class should store
 * material index, object index, and triangle index to work with. In this case,
 * each triangle has to be constructed for every iteration inside the
 * GetTriangleIntersection function, for every call. This results in a lot of
 * repetitive work, but saves resources.
 */


//void interactiveRender(Params& params) {
//    iniData interactiveData;
//    Scene scene( "./rsc/Pyramid.crtscene" );
//    scene.ParseSceneFile();
//
//    while ( true ) {
//        interactiveData = readConfig();
//
//        render( scene );
//    }
//}


int main() {
    Camera camera{};

    //params.camera.Dolly( -3 );
    //params.camera.Pan( 45 );
    //params.camera.Dolly( 3 );
    //params.camera.Tilt( 15 );
    //params.camera.RotateAroundPoint( {0, 0, -3}, {0, 45, 0});

    Scene scene( "./rsc/scene1.crtscene" );
    //Scene scene( "./rsc/RefractionBall.crtscene" );
    //Scene scene( "./rsc/TestPlane.obj" );
    scene.SetRenderMode( RenderMode::Material );
    scene.ParseSceneFile();
    //scene.ParseObjFile();

    Render render( scene );

    double secondsMedian{};
    int nrRuns{ 1 };
    for ( int i{}; i < nrRuns; ++i ) {
        std::chrono::high_resolution_clock::time_point start{
            std::chrono::high_resolution_clock::now() };

        //render.RenderImage();
        render.RenderBuckets();

        std::chrono::high_resolution_clock::time_point stop{
            std::chrono::high_resolution_clock::now() };

        std::chrono::microseconds duration{
            std::chrono::duration_cast<std::chrono::microseconds>(stop - start) };
        const double seconds{ duration.count() / 1'000'000.0 };
        secondsMedian += seconds;

        std::cout << "Execution time: " << seconds << " seconds." << std::endl;
    }

    std::cout << "\n\nMedian execution time: " << secondsMedian / nrRuns <<
        " for " << nrRuns << " runs." << std::endl;

    //renderCameraMoveAnimation( scene, { 0.f, 1.f, 5.f }, { 0.2f, 0.f, 0.f }, 10 );
    //renderRotationAroundObject( scene, { 0.f, 2.f, 5.f }, 30 );

    //renderPyramid( params );
    //renderRotationAroundPyramid( params );

    //renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

    //interactiveRender( params );

    return 0;
}
