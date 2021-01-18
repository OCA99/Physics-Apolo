#pragma once

#include "Vector2.h"

template<class TYPE>
struct Rect {
	Vector2<TYPE> min;
	Vector2<TYPE> max;

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
};

typedef Rect<int> Recti;
typedef Rect<float> Rectf;