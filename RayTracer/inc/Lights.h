#ifndef LIGHTS_H
#define LIGHTS_H

#include "Vectors.h" // FVector3

class Light {
public:
	// @param[in] pos: A point in world space where the light is located.
	// @param[in] intensity: The intensity of the light in generic units.
	Light( const FVector3& pos, const float intensity )
		: m_position{ pos }, m_intensity{ intensity } {
	}

	virtual ~Light() = default;

	// Get the light's position.
	// @return The point in space where this light is located.
	const FVector3& GetPosition() const { return m_position; }
	// Get the light's intensity.
	// @return The intensity of this light in generic units.
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