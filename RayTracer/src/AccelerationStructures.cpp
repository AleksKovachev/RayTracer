#include "AccelerationStructures.h"
#include "RenderMode.h" // RenderMode
#include "RenderSettings.h" // IntersectionData
#include "Rays.h" // Ray, RayType
#include "Scene.h" // Scene
#include "utils.h" // areEqual, isGE, isLT

#include <algorithm> // min, max
#include <limits> // <float>::min, <float>::max
#include <stdexcept> // out_of_range


AABBox::AABBox() :
	min{ std::numeric_limits<float>::max(),
		 std::numeric_limits<float>::max(),
		 std::numeric_limits<float>::max() },
	max{ std::numeric_limits<float>::lowest(),
		 std::numeric_limits<float>::lowest(),
		 std::numeric_limits<float>::lowest() } {
}

AABBox::AABBox( const FVector3& min, const FVector3& max ) : min{ min }, max{ max } {}

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

AccTreeNode::AccTreeNode( const AABBox& aabb, const Scene& scene )
	: aabb{ aabb }, m_scene{ scene } {
}

void AccTreeNode::Intersect(
	const Ray& ray,
	const float maxT,
	float& closestIntersection,
	IntersectionData& data
) const {
	float bias{ 0.f }; //! Default value currently only used for reflection!

	if ( ray.type == RayType::Shadow )
		bias = m_scene.settings.shadowBias;
	else if ( ray.type == RayType::Refractive )
		bias = m_scene.settings.refractBias;

	for ( const int triIdx : triIndices ) {
		const Triangle& triangle = m_scene.GetTriangles()[triIdx];
		// Skip Refractive materials with Shadow Rays
		if ( ray.type == RayType::Shadow
			&& m_scene.GetMaterials()[triangle.matIdx].type == MaterialType::Refractive ) {
			continue;
		}

		float rayProj = ray.direction.Dot( triangle.GetNormal() );

		if ( ray.ignoreBackface ) {
			if ( isGE( rayProj, 0.f ) )
				continue;
		} else {
			// If rayProj > 0, ray points towards triangle back face. < 0 - its back.
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

		data.filled = true;

		// For shadow ray it's enough to have a single intersection.
		if ( ray.type == RayType::Shadow )
			return;

		closestIntersection = rayPointDist;
		data.faceNormal = triangle.GetNormal();
		data.hitPoint = intersectionPt;
		if ( m_scene.settings.renderMode == RenderMode::RandomMeshColor ) {
			data.material = &(m_scene.GetOverrideMaterials()[triangle.overrideMatIdx]);
		} else {
			data.material = &(m_scene.GetMaterials()[triangle.matIdx]);
		}
		data.triangle = triangle;
	}
}

AccTree::AccTree( const Scene& scene )
	: maxDepth{ -1 }, maxBoxTriangleCount{ -1 }, axisCount{ 3 }, m_scene{ scene } {
}

AccTree::AccTree(
	const Scene& scene,
	const int maxDepth_,
	const int maxBoxTriangleCount_,
	const int axisCount_
) :
	maxDepth{ maxDepth_ },
	maxBoxTriangleCount{ maxBoxTriangleCount_ },
	axisCount{ axisCount_ },
	m_scene{ scene } {
}

int AccTree::AddNode( const AABBox& aabb, const Scene& scene ) {
	nodes.emplace_back( aabb, scene );
	return static_cast<int>(nodes.size() - 1);
}

void AccTree::Build( const int nodeIdx, const int depth, std::vector<int>& triIndices ) {
	if ( triIndices.size() <= maxBoxTriangleCount || depth >= maxDepth ) {
		nodes[nodeIdx].triIndices = triIndices;
		return;
	}

	auto [child0AABB, child1AABB] = nodes[nodeIdx].aabb.Split( depth % axisCount );

	std::vector<int> child0triIndices{};
	std::vector<int> child1triIndices{};

	// If any of the triangles intersect with any child's AABB - add the
	// triangles to that child's triangles collection.
	for ( const int triIdx : triIndices ) {
		const Triangle& triangle = m_scene.GetTriangles()[triIdx];
		if ( TriangleIntersectAABB( triangle, child0AABB ) )
			child0triIndices.push_back( triIdx );
		if ( TriangleIntersectAABB( triangle, child1AABB ) )
			child1triIndices.push_back( triIdx );
	}

	// If no triangle intersects with childAABB, the same AABB
	// is used for next iteration with different axis split.

	if ( child0triIndices.size() > 0 ) {
		const int child0Idx = AddNode( child0AABB, m_scene );
		nodes[nodeIdx].children[0] = child0Idx;
		Build( child0Idx, depth + 1, child0triIndices );
	}

	if ( child1triIndices.size() > 0 ) {
		const int child1Idx = AddNode( child1AABB, m_scene );
		nodes[nodeIdx].children[1] = child1Idx;
		Build( child1Idx, depth + 1, child1triIndices );
	}
}

bool AccTree::TriangleIntersectAABB( const Triangle& triangle, AABBox& aabb ) {
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

const std::vector<AccTreeNode>& AccTree::GetNodes() const {
	return nodes;
}

AccTreeNode& AccTree::operator[]( const int idx ) {
	if ( idx > nodes.size() )
		throw std::out_of_range( "Index out of bounds." );
	return nodes[idx];
}
