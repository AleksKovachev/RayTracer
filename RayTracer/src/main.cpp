#include <chrono> // high_resolution_clock, microseconds
#include <iostream> // cout, endl

#include "Camera.h" // Camera
#include "Render.h" // Render, RenderMode
#include "Scene.h" // Scene
#include "Vectors.h" // FVector3

// TODO: Finish the interactive rendering.
// TODO: Finish the OBJ file parser.
// TODO: Implement reading and applying Normal maps.
// TODO: Implement GI and antialiasing.
// TODO: Implement BVH Tree and SAH.
// TODO: Implement bitmap texture mapping without stretching. 2 types - Fit, which
//       puts black bars around the texture to fit the geometry, and Fill, which
//       places the texture making it go "out of bounds" but fill the geometry.


/*
 * The whole Render Engine is built for ease of use, which costs a lot of memory.
 * The program should only work with the Mesh class. The IntersectionData class
 * should store object index, and triangle index to work with. In this case,
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


int main( int argc, char* argv[] ) {
	Camera camera{};

	//params.camera.Dolly( -3 );
	//params.camera.Pan( 45 );
	//params.camera.Dolly( 3 );
	//params.camera.Tilt( 15 );
	//params.camera.RotateAroundPoint( {0, 0, -3}, {0, 45, 0});

	Scene scene( "./rsc/scene1.crtscene" );
	//Scene scene( "./rsc/RefractionBall.crtscene" );
	//Scene scene( "./rsc/TestPlane.obj" );
	scene.settings.renderMode = RenderMode::Material;
	scene.ParseSceneFile();
	//scene.ParseObjFile();

	Render render( scene );

	double secondsMedian{};
	int nrRuns{ 1 };
	for ( int i{}; i < nrRuns; ++i ) {
		std::chrono::high_resolution_clock::time_point start{
			std::chrono::high_resolution_clock::now() };

		render.RenderImage();
		//render.RenderBuckets();

		std::chrono::high_resolution_clock::time_point stop{
			std::chrono::high_resolution_clock::now() };

		std::chrono::microseconds duration{
			std::chrono::duration_cast<std::chrono::microseconds>(stop - start) };
		const double seconds{ duration.count() / 1'000'000.0 };
		secondsMedian += seconds;

		std::cout << "Execution time: " << seconds << " seconds." << std::endl;
	}

	std::cout << "\n\nMean execution time: " << secondsMedian / nrRuns <<
		" for " << nrRuns << " runs." << std::endl;

	//renderCameraMoveAnimation( scene, { 0.f, 1.f, 5.f }, { 0.2f, 0.f, 0.f }, 10 );
	//renderRotationAroundObject( scene, { 0.f, 2.f, 5.f }, 30 );

	//renderPyramid( params );
	//renderRotationAroundPyramid( params );

	//renderCameraTruckAnimation( Pyramid( { 0, 0, 0 } ).shape, params );

	//interactiveRender( params );

	return 0;
}
