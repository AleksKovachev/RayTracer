#include "Camera.h"
#include "Lights.h" // Light, PointLight
#include "Scene.h" // Scene
#include "SpaceConversions.h" // ray2NDC, NDC2ScreenSpace, getFixedAspectRatio
#include "RenderSettings.h" // Settings

#include <algorithm> // round
#include <limits> // numeric_limits<float>::max
#include <numbers> // pi
#include <numeric> // gcd


ImagePlane::ImagePlane( int width, int height, float distFromCamera )
	: resolution{ static_cast<float>(width), static_cast<float>(height) },
	distanceFromCamera{ distFromCamera } {
	init();
}

void ImagePlane::init() {
	int gcd = std::gcd( static_cast<int>(resolution.x),
		static_cast<int>(resolution.y) );
	m_aspectRatio = { resolution.x / gcd, resolution.y / gcd };
}

Camera::Camera() : m_imgPlane{ ImagePlane() } {}

Camera::Camera(
	const ImagePlane& imgPlane, const FVector3& direction, const FVector3& pos ) {
}

Camera::~Camera() {
	for ( Obj* obj : m_children )
		delete obj;
}

FVector3 Camera::GetLocation() const {
	return m_position;
};

void Camera::Move( const FVector3& relPos ) {
	const FVector3 moveDirInWorldSpace{ relPos * m_orientation };

	std::vector<FVector3> childrenRelPos;
	childrenRelPos.reserve( m_children.size() );

	for ( Obj* child : m_children )
		childrenRelPos.push_back( m_position + child->GetLocation() );
		//childrenRelPos.push_back( child->getLocation() - m_location );

	m_position += moveDirInWorldSpace;
	for ( size_t i{}; i < m_children.size(); ++i )
		m_children[i]->Move( relPos + childrenRelPos[i] );
}

Ray Camera::GenerateRay(
	const float x,
	const float y,
	const Settings& settings
) const {
	FVector2 ndcCoords = ray2NDC( x, y, m_imgPlane );
	FVector2 screenCoords = NDC2ScreenSpace( ndcCoords );
	FVector2 fixedAspectRatio = getFixedAspectRatio( screenCoords, m_imgPlane );
	FVector3 rayDirection = FVector3{ fixedAspectRatio, -1.f * m_imgPlane.distanceFromCamera };
	FVector3 finalDirection = ApplyRotation( rayDirection ).Normalize();
	return Ray( m_position, finalDirection, -1, RayType::Camera, settings.ignoreBackface );
}

void Camera::Dolly( const float val ) {
	Move( { 0.f, 0.f, val } );
}

void Camera::Truck( const float val ) {
	Move( { val, 0.f, 0.f } );
}

void Camera::Pedestal( const float val ) {
	Move( { 0.f, val, 0.f } );
}

void Camera::Pan( const float deg ) {
	Rotate( 0.f, deg, 0.f );
}

void Camera::Tilt( const float deg ) {
	Rotate( deg, 0.f, 0.f );
}

void Camera::Roll( const float deg ) {
	Rotate( 0.f, 0.f, deg );
}

void Camera::RotateAroundPoint( const FVector3& pivot, const FVector3& angle ) {
	// Calculate vector from pivot to camera in world space.
	FVector3 relPos{ m_position - pivot };

	// Build combined rotation matrix.
	Matrix3 rotX{ GetXRotMatrix( angle.x ) };
	Matrix3 rotY{ GetYRotMatrix( angle.y ) };
	Matrix3 rotZ{ GetZRotMatrix( angle.z ) };
	Matrix3 rotMat{ rotZ * rotY * rotX };

	// Rotate relative position vector.
	FVector3 rotatedRelPos = relPos * rotMat;

	// Compute new camera position by adding pivot back.
	FVector3 newCameraPos = pivot + rotatedRelPos;

	// Calculate world-space move from current position to new position.
	FVector3 moveVecWorld = newCameraPos - m_position;

	// Convert world - space move vector into camera - local space.
	FVector3 moveVecLocal = moveVecWorld * m_orientation.GetInverse();

	// Move camera to new position and apply rotation.
	Move( moveVecLocal );
	Rotate( angle );
}
