#include "SpaceConversions.h"

FVector2 world2RasterSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { w_coords.x - (imgPlane.resolution.x / 2.f),
             w_coords.y - (imgPlane.resolution.y / 2.f) };
}

FVector2 raster2WorldSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { imgPlane.resolution.x / 2.f + abs( w_coords.x ),
             imgPlane.resolution.y / 2.f + abs( w_coords.y ) };
}

FVector2 raster2NDC( const FVector2& coords, const ImagePlane& imgPlane ) {
    return { coords.x / imgPlane.resolution.x, coords.y / imgPlane.resolution.y };
}

FVector2 ray2NDC( const int x, const int y, const ImagePlane& imgPlane ) {
    return { (x + 0.5f) / imgPlane.resolution.x, (y + 0.5f) / imgPlane.resolution.y };
}

FVector2 NDC2ScreenSpace( const FVector2& coords ) {
    // Inverts Y
    return { (2.f * coords.x) - 1.f, 1.f - (2.f * coords.y) };
    // No-invert version
     //return { (2.f * coords.x) - 1.f, (2.f * coords.y) -1.f };
}

FVector2 ScreenSpace2NDC( const FVector2& coords ) {
    // Inverts Y
    return { (coords.x + 1.f) / 2.f, (1.f - coords.y) / 2.f };
    // No-invert version
    //return { (coords.x + 1.f) / 2.f, (coords.y + 1.f) / 2.f };
}

float ScreenSpace2NDC( float val ) {
    return (val + 1.f) / 2.f;
}

float ScreenSpace2NDCInvert( float val ) {
    return (1.f - val) / 2.f;
}

FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane ) {
    return { vec.x * (imgPlane.resolution.x / imgPlane.resolution.y), vec.y };
}
