#include "Contract.h"

void Contract::ResolvePenetration() {
	if (a->IsStatic() && b->IsStatic()) {
		return;
	}

	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da;
	b->position += normal * db;
}

void Contract::ResolveCollision() {
	ResolvePenetration();

	float e = std::min(a->restitution, b->restitution);

	const Vec2 vrel = a->velocity - b->velocity;

	float vrelDotNormal = vrel.Dot(normal);
	const Vec2 jNormal = normal;
	const float jMag = -(1 + e) * vrelDotNormal / (a->invMass + b->invMass);

	Vec2 j = jNormal * jMag;

	a->AddImpulse(j);
	b->AddImpulse(-j);
}