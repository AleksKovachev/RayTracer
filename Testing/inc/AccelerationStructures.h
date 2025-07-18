#ifndef ACCELERATION_STRUCTURES_H
#define ACCELERATION_STRUCTURES_H

#include <vector>

#include "Triangle.h"
#include "Vectors.h"

struct IntersectionData;
struct Material;
struct Ray;


// Simple structure representing an Axis Aligned Bounding Box.
struct AABBox {
	FVector3 min;
	FVector3 max;

	AABBox();
};

struct AccTreeNode {
	AccTreeNode(
		const AABBox& aabb,
		const int parentIdx,
		const int leftNodeIdx,
		const int rightNodeIdx,
		const std::vector<Triangle>& triangles
	);

	// Intersect the given ray with the triangles in the box (leaf nodes).
	void Intersect(
		const Ray& ray,
		const float maxT,
		const std::vector<Material>& materials,
		float& t,
		float& minT,
		IntersectionData& data
	) const;

	std::vector<Triangle> triangles; // Use for intersection in leaf nodes.
	AABBox aabb; // Axis Aligned Bounding Box for the sub space this node represents.
	// The left and right indices for the node's children,
	// indexing in the big list for the tree with all the nodes.
	int children[2];
	int parent; // The index of the parent node for this node and its sub space.
};

#endif // ACCELERATION_STRUCTURES_H