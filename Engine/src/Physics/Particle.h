#include "Vec2.h"
#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	float mass;
	float invMass;

	Vec2 resultantForces;

	Particle(float x, float y, float mass);
	~Particle();

	void Integrate(float dt);

	void AddForce(const Vec2& force);
	
	void ClearForces();

};

#endif