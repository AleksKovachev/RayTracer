#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "Bases.h"
#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"

#include <vector>


std::vector<Triangle> getGround() {
	std::vector<Triangle> shape;
	Color groundColor;

	// Ground
	float groundSize{ 10.f };
	shape.emplace_back(
		FVector3{ -1.f, 0.f, -1.f } *groundSize,
		FVector3{ -1.f, 0.f,  1.f } *groundSize,
		FVector3{ 1.f, 0.f, -1.f } *groundSize
	);
	shape.emplace_back(
		FVector3{ 1.f, 0.f, -1.f } *groundSize,
		FVector3{ -1.f, 0.f,  1.f } *groundSize,
		FVector3{ 1.f, 0.f,  1.f } *groundSize
	);
	shape[0].color = Color( groundColor );
	shape[1].color = Color( groundColor );

	// Ground markers
	float centerMarkerSize{ 0.2f };
	float markerSize{ 0.1f };
	int colorCount = 3;
	shape.emplace_back(
		FVector3{ 1.f, 0.f, 0.f } *centerMarkerSize,
		FVector3{ 0.f, 1.f, 0.f } *centerMarkerSize,
		FVector3{ -1.f, 0.f, 0.f } *centerMarkerSize
	);

	for ( int i{}; i < 5; ++i ) {
		float pt = (i + 1.f) * 10.f;
		shape.emplace_back(
			FVector3{ -pt + 1.f, 0.f, 0.f } *markerSize,
			FVector3{ -pt, 1.f, 0.f } *markerSize,
			FVector3{ -pt - 1.f, 0.f, 0.f } *markerSize
		);
		shape.emplace_back(
			FVector3{ pt + 1.f, 0.f, 0.f } *markerSize,
			FVector3{ pt, 1.f, 0.f } *markerSize,
			FVector3{ pt - 1.f, 0.f, 0.f } *markerSize
		);
		shape.emplace_back(
			FVector3{ 1.f, 0.f, -pt } *markerSize,
			FVector3{ 0.f, 1.f, -pt } *markerSize,
			FVector3{ -1.f, 0.f, -pt } *markerSize
		);
		colorCount += 3;
	}
	return shape;
}

struct Pyramid : Obj {
	std::vector<Triangle> shape;
	Color baseColor;
	Color groundColor;
	Color frontFace;
	Color sideFace;

	virtual FVector3 getLocation() const override {
		return m_position;
	}

	virtual void move( const FVector3& vec ) override {
		m_position = vec;
		for ( Triangle& triangle : shape ) {
			triangle.move( vec );
		}
	}

	Pyramid( FVector3 position )
		: baseColor{ 60, 40, 60 }, frontFace{ 220, 150, 0 }, sideFace{ 90, 80, 20 }, groundColor{ 60, 60, 60 }, Obj( position ) {

		// Walls
		float X{ 0.f };
		float Y{ 0.f };
		float Z{ 0.f };
		shape.emplace_back(
			FVector3{ 0.f + X, 1.52f + Y, -3.f + Z } + m_position,
			FVector3{ -0.11f + X, -0.07f + Y, -1.12f + Z } + m_position,
			FVector3{ 1.43f + X, -0.27f + Y, -2.38f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 1.52f + Y, -3.f + Z } + m_position,
			FVector3{ -1.37f + X, -0.55f + Y, -2.6f + Z } + m_position,
			FVector3{ -0.11f + X, -0.07f + Y, -1.12f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 1.52f + Y, -3.f + Z } + m_position,
			FVector3{ 0.17f + X, -0.75f + Y, -3.86f + Z } + m_position,
			FVector3{ -1.37f + X, -0.55f + Y, -2.6f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 1.52f + Y, -3.f + Z } + m_position,
			FVector3{ 1.43f + X, -0.27f + Y, -2.38f + Z } + m_position,
			FVector3{ 0.17f + X, -0.75f + Y, -3.86f + Z } + m_position
		);
		// Base
		shape.emplace_back(
			FVector3{ 1.43f + X, -0.27f + Y, -2.38f + Z } + m_position,
			FVector3{ -1.37f + X, -0.55f + Y, -2.6f + Z } + m_position,
			FVector3{ 0.17f + X, -0.75f + Y, -3.86f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 1.43f + X, -0.27f + Y, -2.38f + Z } + m_position,
			FVector3{ -0.11f + X, -0.07f + Y, -1.12f + Z } + m_position,
			FVector3{ -1.37f + X, -0.55f + Y, -2.6f + Z } + m_position
		);

		shape[0].color = Color( frontFace );
		shape[1].color = Color( sideFace );
		shape[2].color = Color( 0, 0, 255 ); // Back Face
		shape[3].color = Color( 255, 255, 0 ); // Back Face
		shape[4].color = Color( { 0, 255, 0 } );
		shape[5].color = Color( baseColor );
	}

};

#endif // SCENEOBJECTS_H