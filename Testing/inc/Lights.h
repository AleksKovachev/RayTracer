#ifndef LIGHTS_H
#define LIGHTS_H

#include "Vectors.h"

class Light {
public:
	Light( const FVector3& pos )
		: m_position{ pos } {
	}

	virtual ~Light() = default;

	const FVector3& getPosition() const { return m_position; }

protected:
	FVector3 m_position; // World space coordinates
};

class PointLight : public Light {
public:
	PointLight( const FVector3& pos )
		: Light( pos ) {}
};

#endif // LIGHTS_H