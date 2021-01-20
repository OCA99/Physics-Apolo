#pragma once

class Polygon;
class Circle;

#include "DynArray.h"
#include "Rect.h"

class Shape
{
public:

	enum class Type {
		UNDEFINED,
		POLYGON,
		CIRCLE
	};

	Shape()
	{

	}

	~Shape()
	{

	}

	virtual void ComputeAABB() = 0;
	virtual void Translate(Vec2f delta) = 0;
	virtual void RotateAround(float angle, Vec2f pivot) = 0;
	virtual bool Intersects(Polygon* other, DynArray<Vec2f>& points) = 0;
	virtual bool Intersects(Circle* other, Vec2f& point) = 0;


	Type type = Type::UNDEFINED;
	Rectf* AABB = nullptr;
};