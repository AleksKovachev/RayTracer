#include "AccelerationStructures.h"

#include <limits> // <float>::min, <float>::max


AccTreeNode::AccTreeNode(
	const AABBox& aabb,
	const int parentIdx,
	const int leftNodeIdx,
	const int rightNodeIdx,
	const std::vector<Triangle>& triangles
) : children{ -1, -1 }, parent{ -1 } {
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
	max{ std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest() } {
}
