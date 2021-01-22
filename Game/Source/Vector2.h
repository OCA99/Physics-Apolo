#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "Defs.h"
#include "Line.h"
#include "DynArray.h"

#include <math.h>

template<class TYPE>
class Vector2
{
public:

	TYPE x, y;

	Vector2()
	{}

	Vector2(const Vector2<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	Vector2(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2& Create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	Vector2 operator -(const Vector2& v) const
	{
		Vector2 r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Vector2 operator + (const Vector2& v) const
	{
		Vector2 r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const Vector2& operator -=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Vector2& operator +=(const Vector2& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	Vector2 operator * (const float k) const
	{
		Vector2 r;

		r.x = x * k;
		r.y = y * k;

		return(r);
	}

	Vector2 operator / (const float k) const
	{
		Vector2 r;

		r.x = x / k;
		r.y = y / k;

		return(r);
	}

	const Vector2& operator *=(const float k)
	{
		x *= k;
		y *= k;

		return(*this);
	}

	const Vector2& operator /=(const float k)
	{
		x /= k;
		y /= k;

		return(*this);
	}

	bool operator ==(const Vector2& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Vector2& v) const
	{
		return (x != v.x || y != v.y);
	}

	static float Determinant(Vector2 a, Vector2 b)
	{
		return a.x * b.y - a.y * b.x;
	}

	static int Orientation(Vector2 p, Vector2 q, Vector2 r)
	{
		int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
		if (val == 0)
			return 0;
		return (val > 0) ? 1 : 2;
	}

	float Angle(Vector2 other)
	{
		return atan2(other.y - y, other.x - x);
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}

	static bool EdgeIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& result)
	{
		float det = Determinant(b - a, c - d);
		float t = Determinant(c - a, c - d) / det;
		float u = Determinant(b - a, c - a) / det;
		if (det == 0)
			return false;
		if (t < 0 || u < 0 || t > 1 || u > 1)
		{
			return false;
		}
		{
			result = a * (1 - t) + b * t;
			return true;
		}
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	Vector2& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Vector2& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	float dot(const Vector2& v)
	{
		return v.x * x + v.y * y;
	}

	static Vector2<TYPE> fitPoints(const DynArray<Vector2<TYPE>>* points)
	{
		Line l;

		int n = points->Count();
		if (n < 2)
			return Vector2<TYPE>(0, 0);

		float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
		for (int i = 0; i < n; i++)
		{
			Vector2<TYPE> p = *points->At(i);
			sumX += p.x;
			sumY += p.y;
			sumXY += p.x * p.y;
			sumX2 += p.x * p.x;
		}

		float xMean = sumX / n;
		float yMean = sumY / n;
		float denominator = sumX2 - sumX * xMean;

		if (abs(denominator) < 1e-4)
		{
			return Vector2<TYPE>(0, 1);
		}

		l.slope = (sumXY - sumX * yMean) / denominator;
		l.y = yMean - l.slope * xMean;

		Vector2 v = Vector2<TYPE>(1, l.evaluate(1)) - Vector2<TYPE>(0, l.evaluate(0));

		return v / v.Length();
	}

	static Vector2<TYPE> average(const DynArray<Vector2<TYPE>>* points)
	{
		Vector2<TYPE> avg = Vector2<TYPE>(0, 0);

		for (int i = 0; i < points->Count(); i++)
		{
			avg += *points->At(i);
		}

		return avg / points->Count();
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Vector2& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx * fx) + (fy * fy));
	}

	TYPE DistanceNoSqrt(const Vector2& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx * fx) + (fy * fy);
	}

	TYPE DistanceManhattan(const Vector2& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}
};

typedef Vector2<int> Vec2i;
typedef Vector2<float> Vec2f;

#endif // __POINT_H__