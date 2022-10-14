#include "Particle.h"
#include "iostream"

// TODO:

Particle::Particle(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;

	if (mass != 0.0) {
		this->invMass = 1 / mass;
	}
	else {
		this->invMass = 0.0;
	}

	std::cout << "Particle constructor!" << std::endl;
}

Particle::~Particle()
{
	std::cout << "Particle destructor!" << std::endl;
}


void Particle::Integrate(float dt) {

	acceleration = resultantForces * invMass;

	velocity += acceleration * dt;
	position += velocity * dt;

	ClearForces();
}

void Particle::AddForce(const Vec2& force) {
	resultantForces += force;
}

void Particle::ClearForces() {
	resultantForces = Vec2();
}
