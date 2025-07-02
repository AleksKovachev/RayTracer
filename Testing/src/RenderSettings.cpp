#include "RenderSettings.h"

Settings::Settings( const int colorDepth )
	: renderWidth{}, renderHeight{}, maxColorComp{ (1 << colorDepth) - 1 },
	BGColor{}, saveDir{ "renders" }, colorMode{ ColorMode::RandomTriangleColor } {
}
