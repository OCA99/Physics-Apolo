#pragma once

#include "DynArray.h"
#include "Vector2.h"
#include "Defs.h"
#include "Rect.h"

#include <math.h>
#include <limits.h>

class Polygon
{
public:
	Polygon(DynArray<Vec2f>* _vertices)
	{
		vertices = _vertices;
	}

	~Polygon()
	{
		delete vertices;
		delete AABB;
		delete centroid;
	}

	void ComputeAABB()
	{
		if (AABB == nullptr)
			AABB = new Rectf(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
		else
		{
			AABB->min.x = INT_MAX;
			AABB->min.y = INT_MAX;
			AABB->max.x = INT_MIN;
			AABB->max.y = INT_MIN;
		}

		for (int i = 0; i < vertices->Count(); i++)
		{
			if (vertices->At(i)->x < AABB->min.x)
				AABB->min.x = vertices->At(i)->x;

			if (vertices->At(i)->y < AABB->min.y)
				AABB->min.y = vertices->At(i)->y;

			if (vertices->At(i)->x > AABB->max.x)
				AABB->max.x = vertices->At(i)->x;

			if (vertices->At(i)->y > AABB->max.y)
				AABB->max.y = vertices->At(i)->y;
		}
	}

	bool Intersects(Polygon* other, Vec2f& point, int& intersections)
	{
		point.x = 0;
		point.y = 0;
		intersections = 0;

		for (int i = 0; i < vertices->Count(); i++)
		{
			for (int j = 0; j < other->vertices->Count(); j++)
			{
				Vec2f result;

				Vec2f a;
				Vec2f b;
				Vec2f c;
				Vec2f d;

				if (i == vertices->Count() - 1)
				{
					a = *vertices->At(i);
					b = *vertices->At(0);
				}
				else
				{
					a = *vertices->At(i);
					b = *vertices->At(i + 1);
				}

				if (j == other->vertices->Count() - 1)
				{
					c = *other->vertices->At(j);
					d = *other->vertices->At(0);
				}
				else
				{
					c = *other->vertices->At(j);
					d = *other->vertices->At(j + 1);
				}

				if (Vec2f::EdgeIntersection(a, b, c, d, result))
				{
					point += result;
					intersections++;
				}
			}
		}

		point /= intersections;

		return (intersections) ? true : false;
	}

	void Translate(Vec2f delta)
	{
		for (int i = 0; i < vertices->Count(); i++)
		{
			vertices->At(i)->x += delta.x;
			vertices->At(i)->y += delta.y;
		}

		ComputeAABB();
		ComputeCentroid();
	}

	void RotateAround(float angle, Vec2f pivot)
	{
		for (int i = 0; i < vertices->Count(); i++)
		{
			float x = (vertices->At(i)->x - pivot.x);
			float y = (vertices->At(i)->y - pivot.y);

			vertices->At(i)->x = (cos(angle) * x - sin(angle) * y) + pivot.x;
			vertices->At(i)->y = (sin(angle) * x + cos(angle) * y) + pivot.y;
		}

		ComputeAABB();
		ComputeCentroid();
	}

	void ComputeCentroid()
	{
		if (centroid == nullptr)
		{
			centroid = new Vec2f(0.0f, 0.0f);
		}
		else
		{
			centroid->x = 0.0f;
			centroid->y = 0.0f;
		}

		for (int i = 0; i < vertices->Count(); i++)
		{
			centroid->x += vertices->At(i)->x;
			centroid->y += vertices->At(i)->y;
		}

		*centroid /= vertices->Count();
	}

	DynArray<Vec2f>* vertices = nullptr;
	Rectf* AABB = nullptr;
	Vec2f* centroid = nullptr;
private:
};