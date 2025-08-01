#include "SpaceConversions.h"
#include "Camera.h"
#include "Vectors.h"


FVector2 ray2NDC( const float x, const float y, const ImagePlane& imgPlane ) {
	return { x / imgPlane.resolution.x, y / imgPlane.resolution.y };
}

FVector2 NDC2ScreenSpace( const FVector2& coords ) {
	// Inverts Y
	return { (2.f * coords.x) - 1.f, 1.f - (2.f * coords.y) };
	// No-invert version
	 //return { (2.f * coords.x) - 1.f, (2.f * coords.y) -1.f };
}

FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane ) {
	return { vec.x * (imgPlane.resolution.x / imgPlane.resolution.y), vec.y };
}
