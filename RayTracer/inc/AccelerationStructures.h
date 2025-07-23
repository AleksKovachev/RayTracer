#ifndef ACCELERATION_STRUCTURES_H
#define ACCELERATION_STRUCTURES_H

#include <utility> // pair
#include <vector> // vector

#include "Triangle.h" // Triangle
#include "Vectors.h" // FVector3

struct IntersectionData;
struct Material;
struct Ray;
class Scene;

// Simple structure representing an Axis Aligned Bounding Box.
struct AABBox {
	FVector3 min;
	FVector3 max;

	AABBox();
	AABBox( const FVector3&, const FVector3& );

	std::pair<AABBox, AABBox> Split( const int splitAxis );
};

struct AccTreeNode {
	AccTreeNode(
		const AABBox& aabb,
		const int leftNodeIdx,
		const int rightNodeIdx,
		const std::vector<Triangle>& triangles,
		const Scene& scene
	);

	// Intersect the given ray with the triangles in the box (leaf nodes).
	void Intersect(
		const Ray& ray,
		const float maxT,
		const std::vector<Material>& materials,
		float& minT,
		IntersectionData& data
	) const;

	std::vector<Triangle> triangles; // Used for intersection in leaf nodes.
	AABBox aabb; // Axis Aligned Bounding Box for the sub space this node represents.
	// The left and right indices for the node's children,
	// indexing in the big list for the tree with all the nodes.
	int children[2];

private:
	const Scene& m_scene;
};


class AccTree {
public:
	std::vector<AccTreeNode> nodes;
	int maxDepth;
	int maxBoxTriangleCount;
	int axisCount;

	AccTree( const Scene& );
	AccTree(
		const Scene& scene,
		const int maxDepth_,
		const int maxBoxTriangleCount_,
		const int axisCount = 3
	);

	int AddNode( const AABBox&, const int, const int, const std::vector<Triangle>&, const Scene& );

	void Build( const int, const int, std::vector<Triangle>& );

	bool TriangleIntersectAABB( const Triangle& triangle, const AABBox& aabb );

	AccTreeNode& operator[]( const int );

private:
	const Scene& m_scene;
};

#endif // ACCELERATION_STRUCTURES_H