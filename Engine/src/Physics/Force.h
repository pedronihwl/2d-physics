#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"
#include "Particle.h"
#include <math.h>

struct Force {
	static Vec2 GenerateDragForce(const Particle& particle, float k);


};

#endif
