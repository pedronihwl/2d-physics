#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
	Vec2 dragForce = Vec2();

	if (particle.velocity.MagnitudeSquared() > 0) {
		float dragMagnitude = k * particle.velocity.MagnitudeSquared();

		dragForce = (particle.velocity.UnitVector() * -1.0) * dragMagnitude;
	}

	return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
	Vec2 frictionForce = Vec2();

	frictionForce = (particle.velocity.UnitVector() * -1) * k;

	return frictionForce;
}

Vec2 Force::GenerateGravitionalForce(const Particle& particleA, const Particle& particleB, float G, float minDistance, float maxDistance) {
	Vec2 distance = particleB.position - particleA.position;

	float distanceMagnitude = std::clamp(distance.MagnitudeSquared(), minDistance, maxDistance);

	float attractionMagnitude = (G * particleA.mass * particleB.mass) / distanceMagnitude;

	return distance.UnitVector() * attractionMagnitude;
		
}

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k) {
	Vec2 distance = particle.position - anchor;

	float displacement = distance.Magnitude() - restLength;

	return distance.UnitVector() * (-k * displacement);
}

Vec2 Force::GenerateSpringForce(const Particle& a, const Particle& b, float restLength, float k) {
	Vec2 distance = a.position - b.position;

	float displacement = distance.Magnitude() - restLength;

	return distance.UnitVector() * (-k * displacement);
}