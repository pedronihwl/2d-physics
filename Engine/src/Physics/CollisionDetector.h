#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "Particle.h"
#include "Contract.h"

struct CollisionDetector {
	static bool IsColliding(Particle* a, Particle* b, Contract& contract);
	static bool IsCollidingCircleCircle(Particle* a, Particle* b, Contract& contract);
	// TODO:: static bool IsCollidingPolygonPolygon(Particle* a, Particle* b);
	// TODO: static bool IsCollidingPolygonCircle(Particle* a, Particle* b);
};

#endif