#ifndef LIGHTS_H
#define LIGHTS_H

#include "Vectors.h" // FVector3

class Light {
public:
	Light( const FVector3& pos, const float intensity )
		: m_position{ pos }, m_intensity{ intensity } {
	}

	virtual ~Light() = default;

	const FVector3& GetPosition() const { return m_position; }
	const float& GetIntensity() const { return m_intensity; }

protected:
	FVector3 m_position; // World space coordinates
	float m_intensity;
};

class PointLight : public Light {
public:
	PointLight( const FVector3& pos, const float intensity )
		: Light( pos, intensity ) {}
};

#endif // LIGHTS_H