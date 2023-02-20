#include "CollisionDetector.h"

bool CollisionDetector::IsColliding(Particle* a, Particle* b, Contract& contract) {

	if (a->shape->GetType() == CIRCLE && b->shape->GetType() == CIRCLE) {
		return CollisionDetector::IsCollidingCircleCircle(a, b, contract);
	}

	if ((a->shape->GetType() == POLYGON || a->shape->GetType() == BOX) 
		&& (b->shape->GetType() == POLYGON || b->shape->GetType() == BOX)) {
		return CollisionDetector::IsCollidingPolygonPolygon(a, b, contract);
	}

	return false;
}


bool CollisionDetector::IsCollidingPolygonPolygon(Particle* a, Particle* b, Contract& contract) {
	Polygon* aPolygon = (Polygon*)a->shape;
	Polygon* bPolygon = (Polygon*)b->shape;

	Vec2 abPoint, baPoint;
	Vec2 abAxis, baAxis;

	float abSeparation = aPolygon->FindMinSeparation(bPolygon, abAxis, abPoint);

	if (abSeparation >= 0) {
		return false;
	}

	float baSeparation = bPolygon->FindMinSeparation(aPolygon, baAxis, baPoint);
	if (baSeparation >= 0) {
		return false;
	}

	return true;
}

bool CollisionDetector::IsCollidingCircleCircle(Particle* a, Particle* b, Contract& contract) {

	Vec2 distance = b->position - a->position;

	float abRadius = ((Circle*)a->shape)->radius + ((Circle*)b->shape)->radius;

	bool isColliding = distance.MagnitudeSquared() <= (abRadius * abRadius);

	if (!isColliding) {
		return false;
	}

	contract.a = a;
	contract.b = b;

	contract.normal = distance;
	contract.normal.Normalize();

	contract.start = b->position - contract.normal * ((Circle*)b->shape)->radius;
	contract.end = a->position + contract.normal * ((Circle*)a->shape)->radius;

	contract.depth = (contract.end - contract.start).Magnitude();

	return true;
}