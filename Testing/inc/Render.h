#ifndef RENDER_H
#define RENDER_H

#include <vector>

class Mesh;
class Scene;
class Triangle;

// Render given scene
void render( const Scene& scene,
	const Camera* overrideCamera = nullptr,
	const std::string* overrideSaveName = nullptr );

/* Render an animation of Camera movement from initial
position with given distance in certain number of frames */
void renderCameraMoveAnimation(
	Scene& scene, const FVector3& initialPos, const FVector3& moveWith, const int frames );

/* Render an animation of Camera orbit around an object in
Z axis from initial position in certain number of frames */
void renderRotationAroundObject(
	const Scene& scene, const FVector3& initialPos, const int frames );


#endif // RENDER_H