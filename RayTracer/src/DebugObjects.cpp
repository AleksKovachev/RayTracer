#include "DebugObjects.h"
#include "Colors.h" // Color

#include "Vectors.h" // FVector3


std::vector<Triangle> getGround() {
	std::vector<Triangle> shape;
	Color groundColor{ 60, 60, 60 };
	int idxColor{};

	// Ground
	float groundSize{ 10.f };
	shape.emplace_back(
		FVector3{ -1.f, 0.f, -1.f } *groundSize,
		FVector3{ -1.f, 0.f,  0.f } *groundSize,
		FVector3{ 1.f, 0.f, -1.f } *groundSize
	);
	shape.emplace_back(
		FVector3{ -1.f, 0.f, 0.f } *groundSize,
		FVector3{ 1.f, 0.f,  1.f } *groundSize,
		FVector3{ 1.f, 0.f,  -1.f } *groundSize
	);
	shape[idxColor++].color = Color( groundColor );
	shape[idxColor++].color = Color( groundColor );

	// Ground markers
	float centerMarkerSize{ 0.2f };
	float markerSize{ 0.1f };
	shape.emplace_back(
		FVector3{ 1.f, 0.f, 0.f } *centerMarkerSize,
		FVector3{ 0.f, 1.f, 0.f } *centerMarkerSize,
		FVector3{ -1.f, 0.f, 0.f } *centerMarkerSize
	);
	shape[idxColor++].color = { 255, 0, 0 };

	for ( int i{}; i < 5; ++i ) {
		float pt = (i + 1.f) * 10.f;
		shape.emplace_back(
			FVector3{ -pt + 1.f, 0.f, 0.f } *markerSize,
			FVector3{ -pt, 1.f, 0.f } *markerSize,
			FVector3{ -pt - 1.f, 0.f, 0.f } *markerSize
		);
		shape[idxColor++].color = { 255, 0, 0 };
		shape.emplace_back(
			FVector3{ pt + 1.f, 0.f, 0.f } *markerSize,
			FVector3{ pt, 1.f, 0.f } *markerSize,
			FVector3{ pt - 1.f, 0.f, 0.f } *markerSize
		);
		shape[idxColor++].color = { 255, 0, 0 };
		shape.emplace_back(
			FVector3{ 1.f, 0.f, -pt } *markerSize,
			FVector3{ 0.f, 1.f, -pt } *markerSize,
			FVector3{ -1.f, 0.f, -pt } *markerSize
		);
		shape[idxColor++].color = { 255, 0, 0 };
	}
	return shape;
}