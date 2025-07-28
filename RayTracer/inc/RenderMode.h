#ifndef RENDER_MODE_H
#define RENDER_MODE_H


// Enum for choosing render style
enum class RenderMode {
	Material, // Render the object with its material.
	RandomMeshColor, // Use a random color for each mesh.
	RandomTriangleColor, // Use a random color for each triangle in the scene.
	Barycentric, // Render object visualizing the Barycentric coordinates.
	Normals, // Render object using normal vectors as colors.
	ShadedNormals // Render object using normal vectors as colors (with shading).
};


#endif // RENDERMODE_H