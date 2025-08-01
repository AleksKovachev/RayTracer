#ifndef SPACECONVERSIONS_H
#define SPACECONVERSIONS_H

struct FVector2;
struct ImagePlane;

// Convert raster coodrinates to NDC(NSC - Normalized Screen Coordinates)[0.0, 1.0]
// Specific for Rays with automatic addition of 0.5 in X and Y for centering a pixel.
// @param[in] x: The x location of the center of the pixel.
// @param[in] y: The y location of the center of the pixel.
// @param[in] imgPlane: The camera's image plane used to get render resolution.
// @return A vector representing the NDC coordinates in [0.0, 1.0] range.
FVector2 ray2NDC( const float, const float, const ImagePlane& );

// Convert NDC to Screen Space (NSC to NDC - Normalized Device Coordinates) [-1.0, 1.0]
// @param[in] coords: A vector with the NDC coordinates in [0.0, 1.0] range.
// @return A vector representing the screen space coordinates in [-1.0, 1.0] range.
FVector2 NDC2ScreenSpace( const FVector2& );

// Apply aspect ratio correction to an FVector2 (pixel).
// @param[in] vec: A vector with the screen space coordinates in range [-1.0, 1.0].
// @param[in] imgPlane:The camera's image plane used to get render resolution.
// @return A vector representing the screen space coordinates after taking
// the aspect ratio into account.
FVector2 getFixedAspectRatio( const FVector2&, const ImagePlane& );

#endif // SPACECONVERSIONS_H