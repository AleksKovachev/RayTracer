#include "Antialiasing.h" // FXAA
#include "ImageBuffer.h" // ImageBuffer
#include "StandaloneOps.h"

#include <fstream> // ofstream


void applyFXAAToImage( Render& render, const std::string& filePath ) {
	ImageBuffer inputIMG{ ImageBuffer::LoadPPM( filePath ) };
	const ImageBuffer* finalImage{ &inputIMG };
	FXAA fxaa{ render.GetScene().settings, &inputIMG};
	finalImage = fxaa.ApplyFXAA();
	const unsigned maxColorComp{
		static_cast<unsigned>((1 << render.GetScene().settings.colorDepth) - 1) };

	std::ofstream ppmFileStream = render.PrepareScene();

	for ( unsigned row{}; row < render.GetScene().settings.renderHeight; ++row ) {
		for ( unsigned col{}; col < render.GetScene().settings.renderWidth; ++col ) {
			writeColorToFile(
				ppmFileStream,
				(*finalImage)[row][col],
				maxColorComp,
				render.GetScene().settings.outputSRGB
			);
		}
	}

	ppmFileStream.close();
}
