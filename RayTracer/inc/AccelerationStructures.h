#ifndef ACCELERATION_STRUCTURES_H
#define ACCELERATION_STRUCTURES_H

#include <utility> // pair
#include <vector> // vector

#include "Triangle.h" // Triangle
#include "Vectors.h" // FVector3

struct IntersectionData;
struct Ray;
class Scene;


// Simple structure representing an Axis Aligned Bounding Box.
struct AABBox {
	FVector3 min;
	FVector3 max;

	// Default constructor that initializes min to <float>::max, and max to <float>::min.
	AABBox();
	// @param[in] min: Value for the minimum component.
	// @param[in] max: Value for the maximum component.
	AABBox( const FVector3&, const FVector3& );

	// Splits the AABB in the middle into two new AABBs.
	// @param[in] splitAxis: The axis along which the AABB will be split in half.
	// @return: A pair of the resulting AABBs.
	std::pair<AABBox, AABBox> Split( const int );
};


// Structure representing a node in the acceleration tree.
struct AccTreeNode {
	// @param[in] aabb: The AABB for this node.
	// @param[in] scene: The scene object with all scene data.
	AccTreeNode( const AABBox&, const Scene& );

	// Intersect the given ray with the triangles in the box (leaf nodes).
	// @param[in] ray: The ray to be intersected with this node's AABB.
	// @param[in] maxT: The maximum distance to check (used with shadow rays).
	// @param[in-out] closestIntersection: The t value of the closest intersected triangle.
	// @param[out] data: The intersection data to be filled for further calculations.
	void Intersect( const Ray&, const float, float&, IntersectionData& ) const;

	std::vector<int> triIndices{}; // Used for intersection in leaf nodes.
	AABBox aabb; // Axis Aligned Bounding Box for the sub space this node represents.
	// The left and right indices for the node's children,
	// indexing in the big list for the tree with all the nodes.
	int children[2]{ -1, -1 };

private:
	// A reference to the Scene object.
	const Scene& m_scene;
};


// Acceleration tree representation.
class AccTree {
public:
	int maxDepth;
	int maxBoxTriangleCount;
	int axisCount;

	// @param[in] scene: The scene object.
	AccTree( const Scene& );
	// @param[in] scene: The scene object.
	// @param[in] maxDepth_: The maximum depth allowed for storing child nodes.
	// @param[in] maxBoxTriangleCount_: The maximum number of triangles allowed
	// to be stored in the AABB.
	// @param[in] axisCount: The number of world axes. Defaults to 3.
	AccTree(
		const Scene& scene,
		const int maxDepth_,
		const int maxBoxTriangleCount_,
		const int axisCount_ = 3
	);

	// Add a node to the Acceleration tree.
	// @param[in] aabb: The AABB for this node's space.
	// @param[in] scene: The scene object.
	// @return The index of the added node.
	int AddNode( const AABBox&, const Scene& );

	// Build the acceleration tree.
	// @param[in] nodeIdx: The index of the node to build.
	// @param[in] depth: The current depth of the build process.
	// @param[in] triIndices: The indices of the triangles to intersect with.
	void Build( const int, const int, std::vector<int>& );

	// Check for intersection between a triangle and an AABB.
	// @param[in] triangle: The triangle to check.
	// @param[in] aabb: The AABB to check.
	// @return If there's an intersection.
	bool TriangleIntersectAABB( const Triangle&, const AABBox& );

	// Gets all of this tree's nodes.
	// @return A collection of nodes.
	const std::vector<AccTreeNode>& GetNodes() const;

	// An access operator to get a node by index.
	// @param[in] idx: The index of the node to get.
	// @return The node at the provided index.
	AccTreeNode& operator[]( const int );

private:
	const Scene& m_scene; // Reference to the project's scene object.
	std::vector<AccTreeNode> nodes; // The actual tree.
};


/* The AccTree class could be ommitted.In such case, the AccTreeNode structure
 * would hold pointers to the children instead of indices. The AccTree member
 * functions would just be free functions. */

#endif // ACCELERATION_STRUCTURES_H