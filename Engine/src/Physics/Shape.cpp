#include "Shape.h"
#include <iostream>
#include <limits>

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

Vec2 Polygon::EdgeAt(int index) const {
	int nextVertice = (index + 1) % worldSpace.size();

	return worldSpace[nextVertice] - worldSpace[index];
}

float Polygon::FindMinSeparation(const Polygon* other, Vec2& axis, Vec2& point) const {
	float separation = std::numeric_limits<float>::lowest();


	for (int i = 0;i < this->worldSpace.size(); i++) {

		Vec2 va = this->worldSpace[i];
		Vec2 normal = this->EdgeAt(i).Normal();

		float minSep = std::numeric_limits<float>::max();

		Vec2 minPoint;
		for (int j = 0;j < other->worldSpace.size(); j++) {
			Vec2 vb = other->worldSpace[j];
			float projection = (vb - va).Dot(normal);

			if (projection < minSep) {
				minPoint = vb;
				minSep = projection;
			}
		}

		if (minSep > separation) {
			separation = minSep;
			axis = this->EdgeAt(i);
			point = minPoint;
		}
	}

	return separation;
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
