#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Colors.h" // Color, ColorMode
#include "Triangle.h"
#include "Vectors.h" // FVector3


// Collection of vertices and vertex indices representing a 3D object
class Mesh {
public:
	Color albedo;

	Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles );

	std::vector<FVector3> GetVertices() const;
	std::vector<int> GetTriangles() const;
private:
	std::vector<FVector3> vertices; // 3D coordinates for the whole mesh
	std::vector<int> triangles; // Each triple of vertices form a triangle
};

// Collection of built Triangles representing a 3D object
class PreparedMesh {
public:
	Color m_albedo;
	std::vector<Triangle> m_triangles;

	void PrepMesh( const Mesh& mesh, const ColorMode& colorMode );

};

#endif // MESH_H