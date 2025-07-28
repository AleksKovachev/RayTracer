#include "SpaceConversions.h"

FVector2 world2RasterSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { w_coords.x - (imgPlane.resolution.x / 2),
             w_coords.y - (imgPlane.resolution.y / 2) };
}

FVector2 raster2WorldSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { imgPlane.resolution.x / 2 + abs( w_coords.x ),
             imgPlane.resolution.y / 2 + abs( w_coords.y ) };
}

FVector2 raster2NDC( const FVector2& coords, const ImagePlane& imgPlane ) {
    return { coords.x / imgPlane.resolution.x, coords.y / imgPlane.resolution.y };
}

FVector2 NDC2ScreenSpace( const FVector2& coords ) {
    // Inverts Y
    return { (2.0 * coords.x) - 1.0, 1.0 - (2.0 * coords.y) };
    // No-invert version
     //return { (2.0 * coords.x) - 1.0, (2.0 * coords.y) -1.0 };
}

FVector2 ScreenSpace2NDC( const FVector2& coords ) {
    // Inverts Y
    return { (coords.x + 1.0) / 2, (1.0 - coords.y) / 2 };
    // No-invert version
    //return { (coords.x + 1.0) / 2, (coords.y + 1.0) / 2 };
}

double ScreenSpace2NDC( double val ) {
    return (val + 1.0) / 2;
}

double ScreenSpace2NDCInvert( double val ) {
    return (1.0 - val) / 2;
}

FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane ) {
    return { vec.x * (imgPlane.resolution.x / imgPlane.resolution.y), vec.y };
}

FVector3 worldSpace2NormalizedVector( const FVector2& vec, const ImagePlane& imgPlane ) {
    FVector3 normRayDir{
        FVector3(
            getFixedAspectRatio(
                NDC2ScreenSpace(
                    raster2NDC(
                        { vec.x, vec.y },
                        imgPlane
                    )
                ), imgPlane
            ), imgPlane.m_position.z
        ).normalize()
    };
    return { ScreenSpace2NDC( { normRayDir.x, normRayDir.y } ), ScreenSpace2NDC( normRayDir.z ) };
}