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
#include "Render.h"
#include "Scene.h"
#include "DebugObjects.h"
#include "SpaceConversions.h"
#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"

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

    Scene scene( "./rsc/scene3.crtscene" );
    //Scene scene( "./rsc/OverlappingTriangles.crtscene" );
    //Scene scene( "./rsc/Pyramid.crtscene" );
    scene.ParseSceneFile();
    render( scene );
    //renderCameraMoveAnimation( scene, { 0.f, 1.f, 5.f }, { 0.2f, 0.f, 0.f }, 10 );
    //renderRotationAroundObject( scene, { 0.f, 2.f, 5.f }, 30 );

    //renderPyramid( params );
    //renderRotationAroundPyramid( params );

    //renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

    //interactiveRender( params );

    return 0;
}
