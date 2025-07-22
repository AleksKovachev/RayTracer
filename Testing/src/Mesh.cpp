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
