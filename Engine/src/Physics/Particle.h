#include "Vec2.h"
#include "Shape.h"
#include <SDL_stdinc.h>
#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	float rotation;
	float angularAcceleration;
	float angularVelocity;

	float mass;
	float invMass;
	float restitution;

	Vec2 resultantForces;
	float sumTorques;

	float I;
	float invI;

	bool hasCollision;

	Uint32 color;

	Shape* shape = NULL;

	Particle(const Shape& shape, float x, float y, float mass, Uint32 color);
	~Particle();

	void Integrate(float dt);

	void AngularIntegration(float dt);

	void AddForce(const Vec2& force);
	
	void AddTorque(const float torque);

	void ClearForces();

	void ClearTorques();

	void UpdateBody(float deltaTime);

	bool IsStatic() const;

	void AddImpulse(const Vec2& J);

};

#endif