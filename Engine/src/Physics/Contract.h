#ifndef CONTRACT_H
#define CONTRACT_H

#include "Particle.h"

struct Contract
{
	Particle* a;
	Particle* b;

	Vec2 start;
	Vec2 end;

	Vec2 normal;
	float depth;

	Contract() = default;
	~Contract() = default;

	void ResolvePenetration();
	void ResolveCollision();
};

#endif