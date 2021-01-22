#pragma once

#include "Vector2.h"
#include "DynArray.h"
#include "Shape.h"
#include "Rect.h"
#include "Log.h"

#include "Polygon.h"

#include <math.h>

class Rigidbody
{
public:
	Rigidbody(Vec2f _position, float _density, int _scale)
	{
		position = _position;
		velocity = Vec2f(0.0f, 0.0f);
		forces = Vec2f(0.0f, 0.0f);
		centerOfMass = _position;
		AABB = new Rectf();
		density = _density;
		scale = _scale;
	}

	~Rigidbody()
	{

	}

	void Update(float dt)
	{
		Vec2f acceleration = forces / mass;
		velocity += acceleration * dt;
		position += velocity;
		float angularAcceleration = torque / momentOfInertia;
		angularVelocity += angularAcceleration * dt;
		angle += angularVelocity;
		UpdateFixtures();
		forces.x = 0.0f;
		forces.y = 0.0f;
		torque = 0.0f;

	}

	void Translate(Vec2f dp)
	{
		for (int i = 0; i < fixtures.Count(); i++)
		{
			(*fixtures.At(i))->Translate(dp);
			CalculateAABB();
		}

		centerOfMass += dp;
	}

	void Rotate(float dr)
	{
		for (int i = 0; i < fixtures.Count(); i++)
		{
			(*fixtures.At(i))->RotateAround(dr, centerOfMass);
			CalculateAABB();
		}
	}

	void UpdateFixtures()
	{
		for (int i = 0; i < fixtures.Count(); i++)
		{
			(*fixtures.At(i))->Translate(velocity);
			(*fixtures.At(i))->RotateAround(angularVelocity, centerOfMass);
			CalculateAABB();
		}

		centerOfMass += velocity;
	}

	void AddForceOnPoint(Vec2f r, Vec2f f)
	{
		AddForce(f);
		AddTorque(r, f);
	}

	void AddForce(Vec2f f)
	{
		forces += f * scale;
	}

	void AddTorque(Vec2f r, Vec2f f)
	{
		Vec2f a = (r - (centerOfMass - position));
		a /= scale;
		torque = a.x * f.y - a.y * f.x;
	}

	void CalculateCenterOfMass()
	{
		centerOfMass = Vec2f(AABB->max.x - ((AABB->max.x - AABB->min.x) / 2), AABB->max.y - ((AABB->max.y - AABB->min.y) / 2));
	}

	void CalculateMass()
	{
		mass = (((AABB->max.x - AABB->min.x) / scale) * ((AABB->max.y - AABB->min.y) / scale) * density);
	}

	void CalculateMomentOfInertia()
	{
		float h = (AABB->max.y - AABB->min.y) / scale;
		float w = (AABB->max.x - AABB->min.x) / scale;

		momentOfInertia = (mass * (h * h + w * w)) / 12;
	}

	void CalculateAABB()
	{
		DynArray<Rectf*>* l = new DynArray<Rectf*>();
		for (int i = 0; i < fixtures.Count(); i++)
		{
			l->PushBack((*fixtures.At(i))->AABB);
		}

		if (AABB != nullptr)
			delete AABB;

		AABB = Rect<float>::JoinRectangles(l);

		CalculateRadius();
	}

	void AddFixture(Shape* s)
	{
		fixtures.PushBack(s);
		s->Translate(position);
		CalculateAABB();
		CalculateMass();
		CalculateCenterOfMass();
		CalculateMomentOfInertia();
	}

	bool Intersects(Rigidbody* other, DynArray<Vec2f>& points)
	{
		for (int i = 0; i < fixtures.Count(); i++)
		{
			for (int j = 0; j < other->fixtures.Count(); j++)
			{
				if ((*fixtures.At(i))->type == Shape::Type::POLYGON && (*other->fixtures.At(j))->type == Shape::Type::POLYGON)
				{
					DynArray<Vec2f> p;
					if (((Polygon*)(*fixtures.At(i)))->Intersects((Polygon*)(*other->fixtures.At(j)), p))
					{
						for (int k = 0; k < p.Count(); k++)
						{
							points.PushBack(*p.At(k));
						}
					}
				}
			}
		}

		return (points.Count()) ? true : false;
	}

	void CalculateRadius()
	{
		Vec2f e = Vec2f(AABB->max.x, AABB->max.y / 2);
		Vec2f w = Vec2f(-AABB->max.x, AABB->max.y / 2);
		Vec2f n = Vec2f(AABB->max.x / 2, AABB->max.y);
		Vec2f s = Vec2f(AABB->max.x / 2, -AABB->max.y);

		radius = (e - centerOfMass).Length();

		if ((w - centerOfMass).Length() > radius)
			radius = (w - centerOfMass).Length();

		if ((n - centerOfMass).Length() > radius)
			radius = (n - centerOfMass).Length();

		if ((s - centerOfMass).Length() > radius)
			radius = (s - centerOfMass).Length();
	}

	float getMoment(Vec2f p)
	{
		float w = abs(angularVelocity) * momentOfInertia;
		float m = mass * velocity.Length();
		return w + m;
	}

	float mass = 1;
	float density = 1;
	float momentOfInertia = 1;
	Vec2f forces;
	float torque;
	Vec2f position;
	Vec2f velocity;
	float angle = 0;
	float angularVelocity = 0;
	Vec2f centerOfMass;
	float radius = 1;

	Rectf* AABB = nullptr;

	DynArray<Shape*> fixtures;

private:
	int scale = 1;
};