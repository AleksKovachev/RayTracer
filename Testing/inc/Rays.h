#ifndef RAYS_H
#define RAYS_H

#include "Vectors.h"


enum class RayType {
	Invalid,
	Camera,     // Ray shot from the camera
	Shadow,     // When shading diffuse material
	Reflective, // When reflective material is hit
	Refractive  // When refractive material is hit
};


struct Ray {
	FVector3 origin;
	FVector3 direction;
	int pathDepth;
	RayType type;
};


#endif // RAYS_H