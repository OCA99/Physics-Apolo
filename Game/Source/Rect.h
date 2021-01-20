#pragma once

#include "Vector2.h"

template<class TYPE>
struct Rect {
	Vector2<TYPE> min;
	Vector2<TYPE> max;

	Rect()
	{
		min.x = 0;
		min.y = 0;
		max.x = 0;
		max.y = 0;
	}

	Rect(TYPE xMin, TYPE yMin, TYPE xMax, TYPE yMax)
	{
		min.x = xMin;
		min.y = yMin;
		max.x = xMax;
		max.y = yMax;
	}

	~Rect()
	{

	}

	bool Intersects(Rect<TYPE>& other)
	{
		float d1x = other->min.x - max.x;
		float d1y = other->min.y - max.y;
		float d2x = min.x - other->max.x;
		float d2y = min.y - other->max.y;

		if (d1x > 0.0f || d1y > 0.0f)
			return false;

		if (d2x > 0.0f || d2y > 0.0f)
			return false;

		return true;
	}

	static Rect<TYPE>* JoinRectangles(DynArray<Rect<TYPE>> l)
	{
		Rect<TYPE>* result = new Rect<TYPE>(INT_MAX, INT_MAX, INT_MIN, INT_MIN);

		for (int i = 0; i < l.Count(); i++)
		{
			Rect<TYPE> r = *l.At(i);
			if (r.min.x < result->min.x)
				result->min.x = r.min.x;
			if (r.min.y < result->min.y)
				result->min.y = r.min.y;
			if (r.max.x > result->max.x)
				result->max.x = r.max.x;
			if (r.max.y > result->max.y)
				result->max.y = r.max.y;
		}

		return result;
	}
};

typedef Rect<int> Recti;
typedef Rect<float> Rectf;