#include "Particle.h"
#include "iostream"

// TODO:

Particle::Particle(const Shape& shape, float x, float y, float mass)
{
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->resultantForces = Vec2(0, 0);
    this->sumTorques = 0.0;
    this->mass = mass;
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