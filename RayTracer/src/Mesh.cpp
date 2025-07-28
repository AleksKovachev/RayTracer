#include "Mesh.h"
#include "RenderSettings.h" // ColorMode
#include "utils.h" // getRandomColor

void PreparedMesh::PrepMesh( const Mesh& mesh, const ColorMode& colorMode ) {

    const std::vector<FVector3>& vertices = mesh.GetVertices();
    size_t vertSize = vertices.size();
    const std::vector<int>& triangles = mesh.GetTriangles();

    for ( size_t triIdx{}; triIdx + 2 < triangles.size(); triIdx += 3 ) {
        // Skip a triangle if it requires a vertex with an invalid index
        bool idx1ok = triangles[triIdx] > vertSize || triangles[triIdx] < 0;
        bool idx2ok = triangles[triIdx + 1] > vertSize || triangles[triIdx + 1] < 0;
        bool idx3ok = triangles[triIdx + 2] > vertSize || triangles[triIdx + 2] < 0;
        if ( idx1ok || idx2ok || idx3ok )
            continue;

        m_triangles.emplace_back( vertices[triangles[triIdx]],
            vertices[triangles[triIdx + 1]],
            vertices[triangles[triIdx + 2]] );

        if ( colorMode == ColorMode::RandomMeshColor )
            m_triangles[m_triangles.size() - 1].color = mesh.albedo;
        else if ( colorMode == ColorMode::RandomTriangleColor )
            m_triangles[m_triangles.size() - 1].color = getRandomColor();
    }
}

Mesh::Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles )
    : vertices{ verts }, triangles{ triangles }, albedo{ 255, 0, 0 } {
}

std::vector<FVector3> Mesh::GetVertices() const {
    return vertices;
}

std::vector<int> Mesh::GetTriangles() const {
    return triangles;
}
