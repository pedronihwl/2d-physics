#include "Vec2.h"
#include "Shape.h"
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

	Vec2 resultantForces;
	float sumTorques;

	float I;
	float invI;

	Shape* shape = NULL;

	Particle(const Shape& shape, float x, float y, float mass);
	~Particle();

	void Integrate(float dt);

	void AngularIntegration(float dt);

	void AddForce(const Vec2& force);
	
	void AddTorque(const float torque);

	void ClearForces();

	void ClearTorques();

	void UpdateBody(float deltaTime);

};

#endif