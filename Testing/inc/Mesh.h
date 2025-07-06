#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Colors.h" // Color, ColorMode
#include "Materials.h" // Material
#include "Triangle.h"
#include "Vectors.h" // FVector3


/* Collection of vertices and vertex indices representing a 3D object. Use this
 * class for large scenes with 10s of thousands of polygons as it's more optimized.
 * Code needs to be reworked to remove all Triangle and Vertex class references.*/
class Mesh {
public:
	Color albedo;

	Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles );

	std::vector<FVector3> GetVertices() const;
	std::vector<int> GetTriangles() const;
	Material GetMaterial() const;
	void SetMaterial( const Material& mat );
	int GetMaterialIdx() const;
	void SetMaterialIdx( const int idx );
private:
	Material material;
	int matIdx;
	std::vector<FVector3> vertices; // 3D coordinates for the whole mesh
	std::vector<int> triangles; // Each triple of vertices form a triangle
	std::vector<FVector3> m_faceNormals;
	std::vector<FVector3> m_vertexNormals;
};

// Collection of built Triangles representing a 3D object
struct PreparedMesh {
	Color m_albedo;
	int m_matIdx;
	Material material;
	std::vector<Triangle> m_triangles;

	void PrepMesh( const Mesh& mesh, const ColorMode& colorMode );
};

#endif // MESH_H