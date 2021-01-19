#pragma once

class Polygon;
class Circle;

class Shape
{
public:
	Shape()
	{

	}

	~Shape()
	{

	}

	virtual void ComputeAABB() = 0;
	virtual void Translate(Vec2f delta) = 0;
	virtual void RotateAround(float angle, Vec2f pivot) = 0;
	virtual bool Intersects(Polygon* other, Vec2f& point, int& intersections) = 0;
	virtual bool Intersects(Circle* other, Vec2f& point) = 0;

	Rectf* AABB = nullptr;
};