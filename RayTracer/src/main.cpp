#include <algorithm>
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

#include "Bases.h"
#include "Camera.h"
#include "DebugObjects.h"
#include "Render.h"
#include "Scene.h"
#include "SpaceConversions.h"
#include "Triangle.h"
#include "utils.h"
#include "Vectors.h"

// TODO: Finish the interactive rendering
// TODO: Finish the OBJ file parser
// TODO: Make the Triangle class hold pointers or indices to the vertices, not their copies.
// TODO: Make the PreparedMesh material property a pointer so materials can be reused.
// TODO: Implement reading and applying Normal maps.
// TODO: Replace IsInShadow() with TraceRay.


/*
The whole Render Engine is built for ease of use, which costs a lot of memory.
To make it more efficient for larger scenes, the PreparedMesh class must be
removed entirely. Teh program should only work with the Mesh class, which should
also store a vector<Material>. Then, the IntersectionData class should store
material index, object index, and triangle index to work with. In this case,
each triangle has to be constructed for every iteration inside the
GetTriangleIntersection function, for every call. This results in a lot of
repetitive work, but saves resources.
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

    Scene scene( "./rsc/RefractionBall.crtscene" );
    //Scene scene( "./rsc/OverlappingTriangles.crtscene" );
    //Scene scene( "./rsc/Pyramid.crtscene" );
    //Scene scene( "./rsc/TestPlane.obj" );
    scene.SetColorMode( ColorMode::LoadedMaterial );
    scene.SetRenderMode( RenderMode::ShadedNormals );
    scene.ParseSceneFile();
    //scene.ParseObjFile();

    Render render( scene );
    render.RenderImage();

    //renderCameraMoveAnimation( scene, { 0.f, 1.f, 5.f }, { 0.2f, 0.f, 0.f }, 10 );
    //renderRotationAroundObject( scene, { 0.f, 2.f, 5.f }, 30 );

    //renderPyramid( params );
    //renderRotationAroundPyramid( params );

    //renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

    //interactiveRender( params );

    return 0;
}
