#ifndef SPACECONVERSIONS_H
#define SPACECONVERSIONS_H

#include "Camera.h"
#include "Vectors.h"

// Convert 2D world coordinates to raster coordinates.
FVector2 world2RasterSpace( const FVector2& w_coords, const ImagePlane& imgPlane );

// Convert raster coordinates to 2D world coordinates.
FVector2 raster2WorldSpace( const FVector2& w_coords, const ImagePlane& imgPlane );

// Convert raster coodrinates to NDC (NSC - Normalized Screen Coordinates) [0.0, 1.0]
FVector2 raster2NDC( const FVector2& coords, const ImagePlane& imgPlane );

// Convert NDC to Screen Space (NSC to NDC - Normalized Device Coordinates) [-1.0, 1.0]
FVector2 NDC2ScreenSpace( const FVector2& coords );

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0]
FVector2 ScreenSpace2NDC( const FVector2& coords );

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0] for a single value
double ScreenSpace2NDC( double val );

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0] for a single value, inverting it
double ScreenSpace2NDCInvert( double val );

// Apply aspect ratio correction to an FVector2 (pixel).
FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane );

/* A common function transforming a pixel from world space
directly to a Normalized Camera Ray Vector. */
FVector3 worldSpace2NormalizedVector( const FVector2& vec, const ImagePlane& imgPlane );

#endif // SPACECONVERSIONS_H