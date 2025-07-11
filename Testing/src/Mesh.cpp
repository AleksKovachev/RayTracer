#include "Mesh.h"
#include "utils.h" // getRandomColor

#include <iostream> // cerr


void PreparedMesh::PrepMesh( const Mesh& mesh, const ColorMode& colorMode ) {

    const std::vector<FVector3>& vertices = mesh.GetVertices();
    size_t vertSize = vertices.size();
    std::vector<FVector3> vertexNormals( vertSize, { 0.f, 0.f, 0.f } );
    std::vector<int> vertexNormalCounts( vertSize, 0 );
    const std::vector<int>& triangles = mesh.GetTriangles();

    for ( size_t triIdx{}; triIdx + 2 < triangles.size(); triIdx += 3 ) {
        int idx0 = triangles[triIdx];
        int idx1 = triangles[triIdx + 1];
        int idx2 = triangles[triIdx + 2];

        // Skip a triangle if it requires a vertex with an invalid index
        if ( idx0 > vertSize || idx0 < 0 ||
            idx1 > vertSize || idx1 < 0 ||
            idx2 > vertSize || idx2 < 0 )
            continue;

        Vertex v0, v1, v2;
        v0.origIdx = idx0;
        v1.origIdx = idx1;
        v2.origIdx = idx2;
        v0.pos = vertices[idx0];
        v1.pos = vertices[idx1];
        v2.pos = vertices[idx2];
        v0.normal = v1.normal = v2.normal = {}; // Init normals to 0

        m_triangles.emplace_back( v0, v1, v2 );
        size_t lastTriIdx = m_triangles.size() - 1;

        vertexNormals[idx0] += m_triangles[lastTriIdx].GetNormal();
        vertexNormals[idx1] += m_triangles[lastTriIdx].GetNormal();
        vertexNormals[idx2] += m_triangles[lastTriIdx].GetNormal();

        vertexNormalCounts[idx0]++;
        vertexNormalCounts[idx1]++;
        vertexNormalCounts[idx2]++;

        if ( colorMode == ColorMode::LoadedMaterial ) {
            const Material& mat = mesh.GetMaterial();
            // Backwards Compatible - needs removal
            m_triangles[lastTriIdx].color = mat.albedo;
            m_material = mat;
        }
        else if ( colorMode == ColorMode::RandomMeshColor ) {
            const Material& mat = mesh.GetMaterialOverride();
            // Backwards Compatible - needs removal
            m_triangles[lastTriIdx].color = mat.albedo;
            m_material = mat;
        }
        else if ( colorMode == ColorMode::RandomTriangleColor ) {
            m_triangles[lastTriIdx].color = getRandomColor();
        }
    }

    // Normalize all Vertex Normals
    for ( size_t i{}; i < vertSize; ++i )
        if ( vertexNormalCounts[i] > 0 )
            vertexNormals[i].NormalizeInPlace();

    for ( Triangle& tri : m_triangles ) {
        tri.SetVertexNormal( 0, vertexNormals[tri.GetVert( 0 ).origIdx] );
        tri.SetVertexNormal( 1, vertexNormals[tri.GetVert( 1 ).origIdx] );
        tri.SetVertexNormal( 2, vertexNormals[tri.GetVert( 2 ).origIdx] );
    }
}

Mesh::Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles )
    : vertices{ verts },
    triangles{ triangles },
    material{ Material( MaterialType::Diffuse, {255, 0, 0 }, false ) },
    matIdx{ -1 } {
}

const std::vector<FVector3>& Mesh::GetVertices() const {
    return vertices;
}

const std::vector<int>& Mesh::GetTriangles() const {
    return triangles;
}

const Material& Mesh::GetMaterial() const {
    return material;
}

void Mesh::SetMaterial( const Material& mat ) {
    material = mat;
}

Material Mesh::GetMaterialOverride() const {
    return m_materialOverride;
}

void Mesh::SetMaterialOverride( const Material& mat ) {
    m_materialOverride = mat;
}

int Mesh::GetMaterialIdx() const {
    return matIdx;
}

void Mesh::SetMaterialIdx( const int idx ) {
    if ( idx < 0 ) {
        std::cerr << "Invalid material index! Not assigning: " << idx << "\n";
        return;
    }
    matIdx = idx;
}
