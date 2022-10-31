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
	this->vertices = vertices;
	std::cout << "Polygon constructor" << std::endl;
}

Polygon::~Polygon() {
	std::cout << "Polygon destructor" << std::endl;
}

Shape* Polygon::Clone() const {
	return new Polygon(this->vertices);
}

ShapeType Polygon::GetType() const {
	return POLYGON;
}

float Polygon::MomentOfInertia() const {
	return 0.0;
}

///

Box::Box(float width, float height) {
	this->width = width;
	this->height = height;
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
