#include "Mesh.h"
#include "utils.h" // getRandomColor

#include <iostream> // cerr


Mesh::Mesh( const std::vector<FVector3>& verts, const std::vector<int>& triangles )
    : vertices{ verts }, triangles{ triangles }, matIdx{ -1 }, overrideMatIdx{ -1 } {
}

const std::vector<FVector3>& Mesh::GetVertices() const {
    return vertices;
}

const std::vector<int>& Mesh::GetTriangles() const {
    return triangles;
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

const std::vector<FVector3>& Mesh::GetTextureUVs() const {
    return m_UVs;
}

void Mesh::SetTextureUVs( const std::vector<FVector3> uvs ) {
    m_UVs = uvs;
}

void PreparedMesh::PrepMesh(
    const Mesh& mesh, const size_t overrideMaterialIdx, const RenderMode& renderMode
) {
    const std::vector<FVector3>& vertices = mesh.GetVertices();
    const std::vector<FVector3>& UVs = mesh.GetTextureUVs();
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
        if ( !UVs.empty() ) {
            v0.UVCoords = UVs[idx0];
            v1.UVCoords = UVs[idx1];
            v2.UVCoords = UVs[idx2];
        }
        v0.normal = v1.normal = v2.normal = {}; // Init normals to 0

        m_triangles.emplace_back( v0, v1, v2 );
        size_t lastTriIdx = m_triangles.size() - 1;

        vertexNormals[idx0] += m_triangles[lastTriIdx].GetNormal();
        vertexNormals[idx1] += m_triangles[lastTriIdx].GetNormal();
        vertexNormals[idx2] += m_triangles[lastTriIdx].GetNormal();

        vertexNormalCounts[idx0]++;
        vertexNormalCounts[idx1]++;
        vertexNormalCounts[idx2]++;

        matIdx = mesh.GetMaterialIdx();
        if ( renderMode == RenderMode::RandomMeshColor ) {
            overrideMatIdx = static_cast<int>( overrideMaterialIdx );
        } else if ( renderMode == RenderMode::RandomTriangleColor ) {
            m_triangles[lastTriIdx].color = getRandomColor();
        }
    }

    // Normalize all Vertex Normals
    for ( size_t i{}; i < vertSize; ++i )
        if ( vertexNormalCounts[i] > 0 )
            vertexNormals[i].NormalizeInPlace();

    for ( Triangle& tri : m_triangles ) {
        tri.SetVertexNormal( 0u, vertexNormals[tri.GetVert( 0u ).origIdx] );
        tri.SetVertexNormal( 1u, vertexNormals[tri.GetVert( 1u ).origIdx] );
        tri.SetVertexNormal( 2u, vertexNormals[tri.GetVert( 2u ).origIdx] );
    }
}
