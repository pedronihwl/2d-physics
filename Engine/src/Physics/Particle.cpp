#include "Particle.h"
#include "iostream"

// TODO:

Particle::Particle(float x, float y, float mass)
{
	this->position = Vec2(x, y);
	this->mass = mass;

	std::cout << "Particle constructor!" << std::endl;
}

Particle::~Particle()
{
	std::cout << "Particle destructor!" << std::endl;
}
