#ifndef STANDALONEOPS_H
#define STANDALONEOPS_H

#include <string> // string

#include "Render.h" // Render
#include "Scene.h" // Scene

// Load a PPM image file and apply FXAA to it.
// @param[in-out] render: The Render object used to get scene settings.
// @param[in] filePath: The path to the PPM file to apply FXAA to.
void applyFXAAToImage( Render& render, const std::string& filePath );

#endif // STANDALONEOPS_H
