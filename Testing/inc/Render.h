#ifndef RENDER_H
#define RENDER_H

#include <vector>

class Mesh;
class Scene;
class Triangle;


class Render {
public:
	const Scene& m_scene;
	Camera* m_overrideCamera;
	const std::string* m_overrideSaveName;
	int m_frames;

	Render( const Scene& scene );
	Render( const Scene& scene, Camera& overrideCamera );
	Render( const Scene& scene, Camera& overrideCamera, const std::string& overrideSaveName );
	~Render();

	void RenderImage();
	void RenderCameraMoveAnimation( const FVector3& initialPos, const FVector3& moveWith );
	void RenderRotationAroundObject( const FVector3& initialPos );

private:
	std::ofstream PrepareScene();

};


#endif // RENDER_H