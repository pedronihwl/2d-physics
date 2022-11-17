#include "Particle.h"
#include "iostream"
#include <math.h>

// TODO:

Particle::Particle(const Shape& shape, float x, float y, float mass, Uint32 color)
{
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->resultantForces = Vec2(0, 0);
    this->restitution = 1.0;
    this->sumTorques = 0.0;
    this->mass = mass;
    this->color = color;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    }
    else {
        this->invMass = 0.0;
    }
    I = shape.MomentOfInertia() * mass;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    }
    else {
        this->invI = 0.0;
    }
    std::cout << "Body constructor called!" << std::endl;
}


Particle::~Particle()
{
	delete shape;
	std::cout << "Particle destructor!" << std::endl;
}


void Particle::Integrate(float dt) {
    if (IsStatic()) return;
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

void Particle::ClearTorques() {
	sumTorques = 0.0;
}

void Particle::AddTorque(const float torque) {
	sumTorques += torque;
}

void Particle::AngularIntegration(float dt) {
    if (IsStatic()) return;

	angularAcceleration = sumTorques * invI;

	angularVelocity += angularAcceleration * dt;
	rotation += angularVelocity * dt;

	ClearTorques();
}

void Particle::UpdateBody(float deltaTime) {
    this->Integrate(deltaTime);
    this->AngularIntegration(deltaTime);

    bool isPolygon = this->shape->GetType() == POLYGON || this->shape->GetType() == BOX;

    if (isPolygon) {
        // Atualizar vertices
        Polygon* polygon = (Polygon*)this->shape;
        polygon->UpdateVertices(this->rotation, this->position);
    }
}

bool Particle::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
}

void Particle::AddImpulse(const Vec2& J) {
    if (this->IsStatic()) return;

    this->velocity += J * this->invMass;
}