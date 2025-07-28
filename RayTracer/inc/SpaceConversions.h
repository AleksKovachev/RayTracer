#ifndef SPACECONVERSIONS_H
#define SPACECONVERSIONS_H

struct FVector2;
struct ImagePlane;


/* Convert raster coodrinates to NDC(NSC - Normalized Screen Coordinates)[0.0, 1.0]
* Specific for Rays with automatic addition of 0.5 in X and Y for centering a pixel */
FVector2 ray2NDC( const int x, const int y, const ImagePlane& imgPlane );

// Convert NDC to Screen Space (NSC to NDC - Normalized Device Coordinates) [-1.0, 1.0]
FVector2 NDC2ScreenSpace( const FVector2& coords );

// Apply aspect ratio correction to an FVector2 (pixel).
FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane );

#endif // SPACECONVERSIONS_H