#include <chrono> // high_resolution_clock, microseconds
#include <iostream> // cout, endl

#include "Antialiasing.h" // Antialiasing, EdgeDetection, FXAA
#include "Camera.h" // Camera
#include "ImageBuffer.h" // ImageBuffer
#include "Render.h" // Render, RenderMode
#include "Scene.h" // Scene
#include "StandaloneOps.h" // applyFXAAToImage
#include "Vectors.h" // FVector3


int main( int argc, char* argv[] ) {
	Camera camera{};

	// Set a file to render.
	Scene scene( "../RayTracer/rsc/scene1.crtscene" );
	//Scene scene( "./rsc/RefractionBall.crtscene" );
	//Scene scene( "./rsc/TestPlane.obj" );

	// Set up render settings.
	scene.settings.renderMode = RenderMode::Material;
	//scene.settings.antialiasing = Antialiasing::SSAA;
	scene.settings.edgeDetectionTypeFXAA = EdgeDetection::LUMA;
	scene.ParseSceneFile();
	//scene.ParseObjFile();
	scene.settings.subPixDepthAA = 1;

	Render render( scene );

	applyFXAAToImage( render, std::string( "./renders/scene1FXAA3x.ppm" ) );

	// Use the render settubgs above to render the scene N times, timing it.
	double secondsMedian{};
	int nrRuns{ 0 };
	for ( int i{}; i < nrRuns; ++i ) {
		std::chrono::high_resolution_clock::time_point start{
			std::chrono::high_resolution_clock::now() };

		//render.RenderImage(); // Single-threadded rendering
		render.RenderBuckets(); // Bucket Rendering

		std::chrono::high_resolution_clock::time_point stop{
			std::chrono::high_resolution_clock::now() };

		std::chrono::microseconds duration{
			std::chrono::duration_cast<std::chrono::microseconds>(stop - start) };
		const double seconds{ duration.count() / 1'000'000.0 };
		secondsMedian += seconds;

		std::cout << "Execution time: " << seconds << " seconds." << std::endl;
	}

	if ( nrRuns > 0 ) {
		std::cout << "\n\nMean execution time: " << secondsMedian / nrRuns <<
			" for " << nrRuns << " runs." << std::endl;
	}

	//render.frames = 10;
	//render.RenderCameraMoveAnimation(
	//	scene.GetCamera().GetLocation(), {0.f, 0.f, 0.2f});
	//render.RenderRotationAroundObject( scene.GetCamera().GetLocation(), { 0.f, 5.f, 0.f } );

	return 0;
}
