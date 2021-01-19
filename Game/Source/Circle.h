#pragma once

#include "Shape.h"
#include "Vector2.h"
#include "Rect.h"
#include <math.h>

class Circle : public Shape
{
public:
	Circle(Vec2f _center, float _radius)
	{
		center = _center;
		radius = _radius;
	}

	~Circle()
	{
		delete AABB;
	}

	void ComputeAABB()
	{
		Vec2f min = Vec2f(center - Vec2f(radius, radius));
		Vec2f max = Vec2f(center + Vec2f(radius, radius));

		AABB = new Rectf(min.x, min.y, max.x, max.y);
	}

	void Translate(Vec2f delta)
	{
		center += delta;
	}

	void RotateAround(float angle, Vec2f pivot)
	{
		float x = center.x - pivot.x;
		float y = center.y - pivot.y;

		center.x = (cos(angle) * x - sin(angle) * y) + pivot.x;
		center.y = (sin(angle) * x + cos(angle) * y) + pivot.y;
	}

	bool Intersects(Polygon* other, Vec2f& point, int& intersections)
	{
		return false;
	}

	bool Intersects(Circle* other, Vec2f& point)
	{
		return false;
	}

	Vec2f center;
	float radius;
}