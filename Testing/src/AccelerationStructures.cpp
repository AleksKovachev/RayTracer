#include "AccelerationStructures.h"
#include "RenderMode.h" // RenderMode
#include "RenderSettings.h" // IntersectionData
#include "Rays.h" // Ray, RayType
#include "Scene.h" // Scene
#include "utils.h" // areEqual, isGE, isLT

#include <algorithm> // min, max
#include <limits> // <float>::min, <float>::max
#include <stdexcept> // out_of_range


AccTreeNode::AccTreeNode(
	const AABBox& aabb,
	const int leftNodeIdx,
	const int rightNodeIdx,
	const std::vector<Triangle>& triangles,
	const Scene& scene
)
	: aabb{ aabb },
	children{ leftNodeIdx, rightNodeIdx },
	triangles{ triangles }, m_scene{ scene } {
}

void AccTreeNode::Intersect(
	const Ray& ray,
	const float maxT,
	const std::vector<Material>& materials, // unused for now
	float& closestIntersection,
	IntersectionData& data
) const {
	float bias{ 0.f }; //! Default value currently only used for reflection!

	if ( ray.type == RayType::Shadow )
		bias = m_scene.GetSettings().shadowBias;
	else if ( ray.type == RayType::Refractive )
		bias = m_scene.GetSettings().refractBias;

	for ( const Triangle& triangle : triangles ) {
		float rayProj = ray.direction.Dot( triangle.GetNormal() );

		if ( ray.ignoreBackface ) {
			if ( isGE( rayProj, 0.f ) )
				continue;
		} else {
			if ( areEqual( rayProj, 0.f ) )
				continue;
		}

		float rayPlaneDist = (triangle.GetVert( 0u ).pos - ray.origin)
			.Dot( triangle.GetNormal() );

		// Ray-to-Point scale factor for unit vector to reach the Point (t).
		float rayPointDist = rayPlaneDist / rayProj;

		if ( ray.ignoreBackface ) {
			if ( isGE( rayPlaneDist, 0.f ) )
				// Ray is not towards Triangle's plane.
				continue;
		} else {
			/* Check if: Ray hits behind the hitPoint(negative rayPointDist)
			 *           Ray hits at or very near the hitPoint (self-intersection)
			 *           Ray hits beyond the light source */
			if ( isLT( rayPointDist, bias ) )
				continue; // This intersection is not valid for shadow casting.
		}

		if ( rayPointDist > maxT || isLE( rayPointDist, 0.f ) )
			continue;

		// Ray parametric equation - represent points on a line going through a Ray.
		FVector3 intersectionPt = ray.origin + (ray.direction * rayPointDist);

		// Ignore intersection if a closer one to the Camera has already been found.
		if ( rayPointDist > closestIntersection )
			continue;

		// If the Plane intersection point is not inside the triangle - don't render it.
		if ( !triangle.IsPointInside( intersectionPt ) )
			continue;

		if ( ray.type == RayType::Shadow )
			data; //TODO: Not finished! Make it work in place of IsInShadow()

		closestIntersection = rayPointDist;
		data.faceNormal = triangle.GetNormal();
		data.hitPoint = intersectionPt;
		if ( m_scene.GetRenderMode() == RenderMode::RandomMeshColor ) {
			data.material = &(m_scene.GetOverrideMaterials()[triangle.overrideMatIdx]);
		} else {
			data.material = &(m_scene.GetMaterials()[triangle.matIdx]);
		}
		data.triangle = triangle;
	}
}

AABBox::AABBox()
	: min{ std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max() },
	max{ std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest(),
		std::numeric_limits<float>::lowest() } {
}

AABBox::AABBox( const FVector3& min, const FVector3& max ) {
	this->min = min;
	this->max = max;
}

std::pair<AABBox, AABBox> AABBox::Split( const int splitAxis ) {
	// Find the middle point of the AABB.
	float mid{ (max[splitAxis] - min[splitAxis]) / 2 };
	float splitPlaneCoord{ min[splitAxis] + mid };

	AABBox A{ min, max };
	AABBox B{ min, max };

	A.max[splitAxis] = splitPlaneCoord;
	B.min[splitAxis] = splitPlaneCoord;

	return { A, B };
}

AccTree::AccTree( const Scene& scene )
	: maxDepth{ -1 }, maxBoxTriangleCount{ -1 }, axisCount{ 3 }, m_scene{ scene } {
}

AccTree::AccTree(
	const Scene& scene,
	const int maxDepth_,
	const int maxBoxTriangleCount_,
	const int axisCount_
)
	: maxDepth{ maxDepth_ },
	maxBoxTriangleCount{ maxBoxTriangleCount_ },
	axisCount{ axisCount_ },
	m_scene{ scene } {
}

int AccTree::AddNode(
	const AABBox& aabb,
	const int child0Idx,
	const int child1Idx,
	const std::vector<Triangle>& triangles,
	const Scene& scene
) {
	nodes.emplace_back( aabb, child0Idx, child1Idx, triangles, scene );
	return static_cast<int>( nodes.size() - 1 );
}

bool AccTree::TriangleIntersectAABB( const Triangle& triangle, const AABBox& aabb ) {
	AABBox triangleAABB;
	for ( int i{}; i < 3; ++i ) {
		triangleAABB.min.x = std::min( triangleAABB.min.x, triangle.GetVert( i ).pos.x );
		triangleAABB.min.y = std::min( triangleAABB.min.y, triangle.GetVert( i ).pos.y );
		triangleAABB.min.z = std::min( triangleAABB.min.z, triangle.GetVert( i ).pos.z );

		triangleAABB.max.x = std::max( triangleAABB.max.x, triangle.GetVert( i ).pos.x );
		triangleAABB.max.y = std::max( triangleAABB.max.y, triangle.GetVert( i ).pos.y );
		triangleAABB.max.z = std::max( triangleAABB.max.z, triangle.GetVert( i ).pos.z );
	}

	for ( int i{}; i < axisCount; ++i ) {
		if ( triangleAABB.min[i] > aabb.max[i] )
			return false;

		if ( triangleAABB.max[i] < aabb.min[i] )
			return false;
	}
	return true;
}

void AccTree::Build( const int nodeIdx, const int depth, std::vector<Triangle>& triangles ) {
	if ( triangles.size() <= maxBoxTriangleCount || depth >= maxDepth ) {
		nodes[nodeIdx].triangles = triangles;
		return;
	}

	auto [child0AABB, child1AABB] = nodes[nodeIdx].aabb.Split( depth % axisCount );

	std::vector<Triangle> child0triangles{};
	std::vector<Triangle> child1triangles{};

	// If any of the triangles intersect with any child's AABB - add the
	// triangles to that child's triangles collection.
	for ( const Triangle& triangle : triangles ) {
		if ( TriangleIntersectAABB( triangle, child0AABB ) )
			child0triangles.push_back( triangle );

		if ( TriangleIntersectAABB( triangle, child1AABB ) )
			child1triangles.push_back( triangle );
	}

	if ( child0triangles.size() > 0 ) {
		const int child0Idx = AddNode( child0AABB, -1, -1, {}, m_scene );
		nodes[nodeIdx].children[0] = child0Idx;
		Build( child0Idx, depth + 1, child0triangles );
	}

	if ( child1triangles.size() > 0 ) {
		const int child1Idx = AddNode( child1AABB, -1, -1, {}, m_scene );
		nodes[nodeIdx].children[1] = child1Idx;
		Build( child1Idx, depth + 1, child1triangles );
	}
}

AccTreeNode& AccTree::operator[]( const int idx ) {
	if ( idx > nodes.size() )
		throw std::out_of_range( "Index out of bounds." );
	return nodes[idx];
}
