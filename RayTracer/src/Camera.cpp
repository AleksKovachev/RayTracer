#include "Camera.h"
#include "Bases.h"
#include "Vectors.h"

#include <vector>

Camera::Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos )
    : Obj( pos ), m_imgPlane{ imgPlane }, m_direction{ direction } {
    m_children.reserve( 1 );
    m_children.push_back( m_imgPlane );
}

void Camera::move( const FVector3& abs_pos ) {
    std::vector<FVector3> childrenRelPos;
    childrenRelPos.reserve( m_children.size() );

    for ( Obj& child : m_children )
        childrenRelPos.push_back( m_position + child.m_position );

    m_position = abs_pos;

    for ( size_t i{}; i < m_children.size(); ++i )
        m_children[i].m_position = abs_pos + childrenRelPos[i];
}

void Camera::moveRel( const FVector3& rel_pos ) {
    m_position += rel_pos;

    for ( Obj& child : m_children ) {
        child.m_position += rel_pos;
    }
}
