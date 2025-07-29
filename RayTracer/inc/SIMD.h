#ifndef SIMD_H
#define SIMD_H

#include <cmath>
#include <immintrin.h>

// Convert 8 float linear components to sRGB using AVX2.
inline __m256 linearToSRGB_8f( __m256 color ) {
    const __m256 threshold = _mm256_set1_ps( 0.0031308f );
    const __m256 scaleLow = _mm256_set1_ps( 12.92f );
    const __m256 scaleHigh = _mm256_set1_ps( 1.055f );
    const __m256 offsetHigh = _mm256_set1_ps( 0.055f );
    const __m256 invGamma = _mm256_set1_ps( 1.f / 2.4f );

    // Mask where color > threshold.
    __m256 mask = _mm256_cmp_ps( color, threshold, _CMP_GT_OS );

    // Compute low part: 12.92 * x.
    __m256 low = _mm256_mul_ps( color, scaleLow );

    // Extract individual floats to apply powf.
    alignas(32) float input[8];
    alignas(32) float highResult[8];
    _mm256_store_ps( input, color );

    for ( int i{}; i < 8; ++i )
        highResult[i] = 1.055f * std::powf( input[i], 1.f / 2.4f ) - 0.055f;

    __m256 high = _mm256_load_ps( highResult );

    // Blend based on mask: if color > threshold, use high; else use low.
    return _mm256_blendv_ps( low, high, mask );
}

// Assumes the image data is stored as contiguous float array [RGBRGBRGB...] (interleaved).
void convertRGBImageToSRGB_AVX( float* data, size_t pixelCount ) {
    const size_t floatsPerPixel = 3; // R, G, B
    const size_t floatsPerBatch = 8; // for AVX2

    size_t totalChannels = pixelCount * floatsPerPixel;

    // Process each channel (R, G, B) separately.
    for ( size_t channelOffset{}; channelOffset < 3; ++channelOffset ) {
        for ( size_t i = channelOffset; i + floatsPerBatch <= totalChannels; i += floatsPerBatch * 3 ) {
            // Gather 8 values with stride 3 (since RGB interleaved).
            alignas(32) float input[8];
            for ( int j = 0; j < 8; ++j )
                input[j] = data[i + j * 3];

            __m256 color = _mm256_load_ps( input );
            __m256 srgb = linearToSRGB_8f( color );

            _mm256_store_ps( input, srgb );

            for ( int j{}; j < 8; ++j )
                data[i + j * 3] = input[j];
        }

        // Handle remainder (scalar fallback).
        for ( size_t i = (totalChannels / (floatsPerBatch * 3)) * (floatsPerBatch * 3) + channelOffset;
            i < totalChannels;
            i += 3 ) {
            float x = data[i];
            data[i] = (x <= 0.0031308f)
                ? 12.92f * x
                : 1.055f * std::powf( x, 1.0f / 2.4f ) - 0.055f;
        }
    }
}


#endif // SIMD_H