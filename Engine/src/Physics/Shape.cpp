#include "Shape.h"
#include <iostream>

Circle::Circle(float radius) {
	this->radius = radius;
	std::cout << "Circle constructor" << std::endl;
}

Circle::~Circle() {
	std::cout << "Circle destructor" << std::endl;
}

Shape* Circle::Clone() const {
	return new Circle(this->radius);
}

ShapeType Circle::GetType() const {
	return CIRCLE;
}

float Circle::MomentOfInertia() const {
	return 0.5 * (radius * radius);
}

////

Polygon::Polygon(std::vector<Vec2> vertices) {
	this->localSpace = vertices;
	std::cout << "Polygon constructor" << std::endl;
}

Polygon::~Polygon() {
	std::cout << "Polygon destructor" << std::endl;
}

Shape* Polygon::Clone() const {
	return new Polygon(this->localSpace);
}

ShapeType Polygon::GetType() const {
	return POLYGON;
}

float Polygon::MomentOfInertia() const {
	return 0.0;
}

void Polygon::UpdateVertices(float angle, const Vec2& position) {

	for (auto i=0; i < localSpace.size(); i++)
	{
		// Rotação
		worldSpace[i] = localSpace[i].Rotate(angle);
		// Translação
		worldSpace[i] += position;
	}
}

///

Box::Box(float width, float height) {
	this->width = width;
	this->height = height;
	
	localSpace.push_back(Vec2(-width / 2.0, -height / 2.0));
	localSpace.push_back(Vec2(+width / 2.0, -height / 2.0));
	localSpace.push_back(Vec2(+width / 2.0, +height / 2.0));
	localSpace.push_back(Vec2(-width / 2.0, +height / 2.0));

	worldSpace.push_back(Vec2(-width / 2.0, -height / 2.0));
	worldSpace.push_back(Vec2(+width / 2.0, -height / 2.0));
	worldSpace.push_back(Vec2(+width / 2.0, +height / 2.0));
	worldSpace.push_back(Vec2(-width / 2.0, +height / 2.0));

	std::cout << "Box constructor" << std::endl;
}

Box::~Box() {
	std::cout << "Box destructor" << std::endl;
}

Shape* Box::Clone() const {
	return new Box(this->width, this->height);
}

ShapeType Box::GetType() const {
	return BOX;
}

float Box::MomentOfInertia() const {
	return (0.083333) * (width * width + height * height);
}
