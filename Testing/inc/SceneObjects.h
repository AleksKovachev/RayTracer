#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "Bases.h"
#include "Triangle.h"
#include "Vectors.h"
#include "utils.h"

#include <vector>

struct Pyramid : Obj {
	std::vector<Triangle> shape;
	Color baseColor;
	Color frontFace;
	Color sideFace;

	virtual FVector3 getLocation() const override {
		return m_location;
	}

	virtual void move( const FVector3& vec ) override {
		m_location = vec;
		for ( Triangle& triangle : shape ) {
			triangle.move( vec );
		}
	}

	Pyramid( FVector3 position )
		: baseColor{ 60, 40, 60 }, frontFace{ 220, 150, 0 }, sideFace{ 90, 80, 20 }, Obj( position ) {
		shape.reserve( 6 );
		// Walls
		shape.emplace_back(
			FVector3{ 0.f, 1.52f, -3.f } + m_location,
			FVector3{ -0.11f, -0.07f, -1.12f } + m_location,
			FVector3{ 1.43f, -0.27f, -2.38f } + m_location
		);
		shape.emplace_back(
			FVector3{ 0.f, 1.52f, -3.f } + m_location,
			FVector3{ -1.37f, -0.55f, -2.6f } + m_location,
			FVector3{ -0.11f, -0.07f, -1.12f } + m_location
		);
		shape.emplace_back(
			FVector3{ 0.f, 1.52f, -3.f } + m_location,
			FVector3{ 0.17f, -0.75f, -3.86f } + m_location,
			FVector3{ -1.37f, -0.55f, -2.6f } + m_location
		);
		shape.emplace_back(
			FVector3{ 0.f, 1.52f, -3.f } + m_location,
			FVector3{ 1.43f, -0.27f, -2.38f } + m_location,
			FVector3{ 0.17f, -0.75f, -3.86f } + m_location
		);
		// Base
		shape.emplace_back(
			FVector3{ 1.43f, -0.27f, -2.38f } + m_location,
			FVector3{ -1.37f, -0.55f, -2.6f } + m_location,
			FVector3{ 0.17f, -0.75f, -3.86f } + m_location
		);
		shape.emplace_back(
			FVector3{ 1.43f, -0.27f, -2.38f } + m_location,
			FVector3{ -0.11f, -0.07f, -1.12f } + m_location,
			FVector3{ -1.37f, -0.55f, -2.6f } + m_location
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