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

bool AABBox::Contains( const CompactFVector3& triCenter ) const {
	return min.x <= triCenter.x && triCenter.x <= max.x
		&& min.y <= triCenter.y && triCenter.y <= max.y
		&& min.z <= triCenter.z && triCenter.z <= max.z;
}

void AABBox::ExpandToInclude( const Triangle& triangle ) {
	const FVector3& v0pos{ triangle.GetVert( 0 ).pos };
	const FVector3& v1pos{ triangle.GetVert( 1 ).pos };
	const FVector3& v2pos{ triangle.GetVert( 2 ).pos };

	min.x = std::min( { min.x, v0pos.x, v1pos.x, v2pos.x } );
	min.y = std::min( { min.y, v0pos.y, v1pos.y, v2pos.y } );
	min.z = std::min( { min.z, v0pos.z, v1pos.z, v2pos.z } );
	max.x = std::max( { max.x, v0pos.x, v1pos.x, v2pos.x } );
	max.y = std::max( { max.y, v0pos.y, v1pos.y, v2pos.y } );
	max.z = std::max( { max.z, v0pos.z, v1pos.z, v2pos.z } );
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
		bias = m_scene.GetSettings().shadowBias;
	else if ( ray.type == RayType::Refractive )
		bias = m_scene.GetSettings().refractBias;

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
		if ( m_scene.GetRenderMode() == RenderMode::RandomMeshColor ) {
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

	// Initialize a new AABB for each child.
	AABBox shrunkAABB0{};
	AABBox shrunkAABB1{};
	bool hasIntersection0{ false };
	bool hasIntersection1{ false };

	// If any of the triangles intersect with any child's AABB - add the
	// triangles to that child's triangles collection.
	for ( const int triIdx : triIndices ) {
		const Triangle& triangle = m_scene.GetTriangles()[triIdx];
		if ( TriangleIntersectAABB( triangle, child0AABB, shrunkAABB0 ) ) {
			child0triIndices.push_back( triIdx );
			hasIntersection0 = true;
		}

		else if ( TriangleIntersectAABB( triangle, child1AABB, shrunkAABB1 ) ) {
			child1triIndices.push_back( triIdx );
			hasIntersection1 = true;
		}
	}

	// If no triangle intersects with childAABB, the same AABB
	// is used for next iteration with different axis split.
	if ( !hasIntersection0 )
		shrunkAABB0 = child0AABB;

	if ( !hasIntersection1 )
		shrunkAABB1 = child1AABB;

	if ( child0triIndices.size() > 0 ) {
		const int child0Idx = AddNode( shrunkAABB0, m_scene );
		nodes[nodeIdx].children[0] = child0Idx;
		Build( child0Idx, depth + 1, child0triIndices );
	}

	if ( child1triIndices.size() > 0 ) {
		const int child1Idx = AddNode( shrunkAABB1, m_scene );
		nodes[nodeIdx].children[1] = child1Idx;
		Build( child1Idx, depth + 1, child1triIndices );
	}
}

bool AccTree::TriangleIntersectAABB(
	const Triangle& triangle, AABBox& childAABB, AABBox& shrunkAABB
) {
	// Find triangle center avoiding FVector3 object construction.
	CompactFVector3 triCenter{ triangle.GetCentroid() };

	// Check for intersection with the AABB.
	bool isIntersecting = childAABB.Contains( triCenter );

	// Fir the whole triangle inside the AABB.
	if ( isIntersecting )
		shrunkAABB.ExpandToInclude( triangle );

	return isIntersecting;
}

const std::vector<AccTreeNode>& AccTree::GetNodes() const {
	return nodes;
}

AccTreeNode& AccTree::operator[]( const int idx ) {
	if ( idx > nodes.size() )
		throw std::out_of_range( "Index out of bounds." );
	return nodes[idx];
}
