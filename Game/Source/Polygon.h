#pragma once

#include "DynArray.h"
#include "Vector2.h"
#include "Defs.h"
#include "Rect.h"
#include "Shape.h"

#include <math.h>
#include <limits.h>

class Polygon : public Shape
{
public:
	Polygon(DynArray<Vec2f>* _vertices) : Shape()
	{
		vertices = _vertices;
		ComputeAABB();
		ComputeCentroid();

		type = Type::POLYGON;
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

	bool Intersects(Polygon* other, DynArray<Vec2f>& points)
	{
		for (int i = 0; i < vertices->Count(); i++)
		{
			for (int j = 0; j < other->vertices->Count(); j++)
			{
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

				Vec2f result;

				if (Vec2f::EdgeIntersection(a, b, c, d, result))
				{
					points.PushBack(result);
				}
			}
		}

		return (points.Count()) ? true : false;
	}

	bool Intersects(Circle* other, Vec2f& point)
	{
		return false;
	}

	Polygon* ConvexHull()
	{
		DynArray<Vec2f>* hull = new DynArray<Vec2f>();

		int n = vertices->Count();

		int l = 0;
		for (int i = 1; i < n; i++)
		{
			if (vertices->At(i)->x < vertices->At(l)->x)
			{
				l = i;
			}
		}

		int p = l;
		int q;
		
		do {
			hull->PushBack(*vertices->At(p));
			q = (p + 1) % n;
			for (int i = 0; i < n; i++)
			{
				if (Vec2f::Orientation(*vertices->At(p), *vertices->At(i), *vertices->At(q)) == 2)
					q = i;
			}
			p = q;
		} while (p != l);

		return new Polygon(hull);
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
	Vec2f* centroid = nullptr;
private:
};