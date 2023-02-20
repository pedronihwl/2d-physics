#include <vector>
#include "Vec2.h"

#ifndef SHAPE_H
#define SHAPE_H

enum ShapeType {
	POLYGON,
	CIRCLE,
	BOX
};

struct Shape {
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
	virtual float MomentOfInertia() const = 0;
};

struct Circle : public Shape {
	float radius;

	Circle(const float radius);

	virtual ~Circle();

	ShapeType GetType() const override;

	Shape* Clone() const override;

	float MomentOfInertia() const override;
};

struct Polygon : public Shape {
	std::vector<Vec2> localSpace;
	std::vector<Vec2> worldSpace;

	Polygon() = default;
	Polygon(const std::vector<Vec2> vertices);

	virtual ~Polygon();

	ShapeType GetType() const override;

	Shape* Clone() const override;

	Vec2 EdgeAt(int index) const;

	float FindMinSeparation(const Polygon* other, Vec2& axis, Vec2& point) const;

	float MomentOfInertia() const override;

	void UpdateVertices(float rotation, const Vec2& position);

};

struct Box : public Polygon {
	float width;
	float height;

	Box(const float width, const float height);

	virtual ~Box();

	ShapeType GetType() const override;

	Shape* Clone() const override;

	float MomentOfInertia() const override;
};


#endif