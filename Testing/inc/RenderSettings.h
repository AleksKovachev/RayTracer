#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include <string>

#include "Bases.h" // Color

enum class ColorMode;

// Global scene settings
struct Settings {
	int renderWidth;
	int renderHeight;
	int maxColorComp;
	Color BGColor;
	std::string saveDir;
	std::string saveName;
	ColorMode colorMode;

	Settings( const int colorDepth = 8 );
};


enum class ColorMode {
	RandomMeshColor,
	RandomTriangleColor
};



#endif // RENDER_SETTINGS_H