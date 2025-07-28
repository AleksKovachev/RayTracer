#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "Bases.h"
#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"

#include <vector>


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
		shape[idxColor++ ].color = { 255, 0, 0 };
		shape.emplace_back(
			FVector3{ 1.f, 0.f, -pt } *markerSize,
			FVector3{ 0.f, 1.f, -pt } *markerSize,
			FVector3{ -1.f, 0.f, -pt } *markerSize
		);
		shape[idxColor++].color = { 255, 0, 0 };
	}
	return shape;
}

struct Pyramid : Obj {
	std::vector<Triangle> shape;
	Color baseColor;
	Color frontFace;
	Color sideFace;

	virtual FVector3 GetLocation() const override {
		return m_position;
	}

	virtual void Move( const FVector3& pos ) override {
		const FVector3 moveDirInWorldSpace{ pos * m_orientation };
		m_position += moveDirInWorldSpace;
		for ( Triangle& triangle : shape ) {
			triangle.Move( pos );
		}
	}

	Pyramid( FVector3 position )
		: baseColor{ 60, 40, 60 }, frontFace{ 220, 150, 0 }, sideFace{ 90, 80, 20 }, Obj( position ) {

		// Walls
		float X{ 0.f };
		float Y{ 0.f };
		float Z{ -2.f };
		shape.emplace_back(
			FVector3{ 0.f + X, 2.27f + Y, -1.07f + Z } + m_position,
			FVector3{ -0.11f + X, 0.68f + Y, 0.81f + Z } + m_position,
			FVector3{ 1.43f + X, 0.48f + Y, -0.45f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 2.27f + Y, -1.07f + Z } + m_position,
			FVector3{ -1.37f + X, 0.2f + Y, -0.67f + Z } + m_position,
			FVector3{ -0.11f + X, 0.68f + Y, 0.81f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 2.27f + Y, -1.07f + Z } + m_position,
			FVector3{ 0.17f + X, 0.f + Y, -1.93f + Z } + m_position,
			FVector3{ -1.37f + X, 0.2f + Y, -0.67f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 0.f + X, 2.27f + Y, -1.07f + Z } + m_position,
			FVector3{ 1.43f + X, 0.48f + Y, -0.45f + Z } + m_position,
			FVector3{ 0.17f + X, 0.f + Y, -1.93f + Z } + m_position
		);
		// Base
		shape.emplace_back(
			FVector3{ 1.43f + X, 0.48f + Y, -0.45f + Z } + m_position,
			FVector3{ -1.37f + X, 0.2f + Y, -0.67f + Z } + m_position,
			FVector3{ 0.17f + X, 0.f + Y, -1.93f + Z } + m_position
		);
		shape.emplace_back(
			FVector3{ 1.43f + X, 0.48f + Y, -0.45f + Z } + m_position,
			FVector3{ -0.11f + X, 0.68f + Y, 0.81f + Z } + m_position,
			FVector3{ -1.37f + X, 0.2f + Y, -0.67f + Z } + m_position
		);

		shape[0].color = Color( frontFace );
		shape[1].color = Color( sideFace );
		shape[2].color = Color( 0, 0, 255 ); // Back Face
		shape[3].color = Color( 0, 255, 0 ); // Back Face
		shape[4].color = Color( baseColor );
		shape[5].color = Color( baseColor );
	}

};

#endif // SCENEOBJECTS_H