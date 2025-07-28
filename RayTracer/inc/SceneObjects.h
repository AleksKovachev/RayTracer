#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"

#include <vector>

std::vector<Triangle> getPyramid() {
	std::vector<Triangle> pyramid;
	pyramid.reserve( 6 );
	// Walls
	pyramid.emplace_back(
		FVector3{ 0, 0.97, -2.18 },
		FVector3{ -1.31, -0.83, -3.2 },
		FVector3{ 0.53, -0.63, -3.96 }
	);
	pyramid.emplace_back(
		FVector3{ 0, 0.97, -2.18 },
		FVector3{ 0.53, -0.63, -3.96 },
		FVector3{ 1.31, -1.1, -2.18 }
	);
	pyramid.emplace_back(
		FVector3{ 0, 0.97, -2.18 },
		FVector3{ 1.31, -1.1, -2.18 },
		FVector3{ -0.53, -1.3, -1.43 }
	);
	pyramid.emplace_back(
		FVector3{ 0, 0.97, -2.18 },
		FVector3{ -0.53, -1.3, -1.43 },
		FVector3{ -1.31, -0.83, -3.2 }
	);
	// Base
	pyramid.emplace_back(
		FVector3{ -1.31, -0.83, -3.2 },
		FVector3{ -0.53, -1.3, -1.43 },
		FVector3{ 1.31, -1.1, -2.18 }
	);
	pyramid.emplace_back(
		FVector3{ -1.31, -0.83, -3.2 },
		FVector3{ 1.31, -1.1, -2.18 },
		FVector3{ 0.53, -0.63, -3.96 }
	);

	for ( Triangle& triangle : pyramid ) {
		triangle.color = Color( getInt( 0, 255 ), getInt( 0, 255 ), getInt( 0, 255 ) );
	}
	pyramid[5].color = pyramid[4].color;

	return pyramid;
}


#endif // SCENEOBJECTS_H