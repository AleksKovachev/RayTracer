#include "AccelerationStructures.h"

#include <limits> // <float>::min, <float>::max


AccTreeNode::AccTreeNode(
	const AABBox& aabb,
	const int parentIdx,
	const int leftNodeIdx,
	const int rightNodeIdx,
	const std::vector<Triangle>& triangles
) {
}

void AccTreeNode::Intersect(
	const Ray& ray,
	const float maxT,
	const std::vector<Material>& materials,
	float& t,
	float& minT,
	IntersectionData& data
) const {
}

AABBox::AABBox()
	: min{ std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max() },
	max{ std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min() } {
}
