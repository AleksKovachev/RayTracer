#ifndef MESH_H
#define MESH_H

#include <vector> // vector

#include "Materials.h" // Material, MaterialType, Texture
#include "RenderMode.h" // RenderMode
#include "Triangle.h" // Triangle, Vertex
#include "Vectors.h" // FVector3


/* Collection of vertices and vertex indices representing a 3D object. Use this
 * class for large scenes with 10s of thousands of polygons as it's more optimized.
 * Code needs to be reworked to remove all Triangle and Vertex class references.*/
class Mesh {
public:
	// @param[in] verts: A collection of vertices that form this mehs.
	// @param[in] triangles: A collection of vertex index tripplets that form triangles.
	Mesh( const std::vector<FVector3>&, const std::vector<int>& );

	// Gets the vertices of this mesh.
	// @return A collection of indices.
	const std::vector<FVector3>& GetVertices() const;

	// Gets the triangles of this mesh.
	// @return A collection vertex index tripplets representing this mesh' triangles.
	const std::vector<int>& GetTriangles() const;

	// Gets the material this mesh' material will be overridden with.
	// @return A material object.
	Material GetMaterialOverride() const;

	// Assigns a material this mesh' material will be overridden with.
	// @param[in] The material object to assign.
	void SetMaterialOverride( const Material& );

	// Returns the material index this object has in the scene file properties.
	// @return An integer index.
	int GetMaterialIdx() const;

	// Sets the material index this object to the one in the scene file properties.
	// @param[in] idx: An integer index.
	void SetMaterialIdx( const int );

	// Gets the UV texture coordinates of this mesh.
	// @return A collection of UV coordinaes.
	const std::vector<FVector3>& GetTextureUVs() const;

	// Assigns UV texture coordinates for this mesh.
	// @param[in] uvs: The collection of UV coordinaes to assign.
	void SetTextureUVs( const std::vector<FVector3> );
private:
	Material m_materialOverride;
	int matIdx;
	int overrideMatIdx;
	std::vector<FVector3> vertices; // 3D coordinates for the whole mesh
	std::vector<int> triangles; // Each triple of vertices form a triangle
	std::vector<FVector3> m_faceNormals;
	std::vector<FVector3> m_vertexNormals;
	std::vector<FVector3> m_UVs;
};

// Collection of built Triangles representing a 3D object
struct PreparedMesh {
	int matIdx{};
	int overrideMatIdx{};
	std::vector<Triangle> m_triangles{};

	// Converts all mesh data inside a mesh object to a prepared mesh object.
	// @param[in] mesh: The mesh to get the data from.
	// @param[in] overrideMaterialIdx: The index of the override material for this mesh.
	// @param[in] renderMode: The chosen color mode to assign correct color.
	void PrepMesh( const Mesh&, const size_t, const RenderMode& );
};

#endif // MESH_H