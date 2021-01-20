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
	Rigidbody(Vec2f _position)
	{
		position = _position;
		velocity = Vec2f(0.0f, 0.0f);
		forces = Vec2f(0.0f, 0.0f);
		centerOfMass = _position;
		AABB = new Rectf();
	}

	~Rigidbody()
	{

	}

	void Update(float dt)
	{
		Vec2f acceleration = forces / mass;
		velocity = acceleration * dt;
		position += velocity;
		float angularAcceleration = torque / momentOfInertia;
		angularVelocity = angularAcceleration * dt;
		angle = angularVelocity;
		UpdateFixtures();
	}

	void UpdateFixtures()
	{
		for (int i = 0; i < fixtures.Count(); i++)
		{
			(*fixtures.At(i))->Translate(velocity);
			CalculateAABB();
			CalculateCenterOfMass();
			(*fixtures.At(i))->RotateAround(angularVelocity, centerOfMass);
		}
	}

	void AddForce(Vec2f r, Vec2f f)
	{
		forces += f;
	}

	void AddTorque(Vec2f r, Vec2f f)
	{
		float a = centerOfMass.Angle(r);
		torque = r.Length() * f.Length() * sin(a);
	}

	void CalculateCenterOfMass()
	{
		centerOfMass = Vec2f(AABB->max.x - (AABB->max.x - AABB->min.x / 2), AABB->max.y - (AABB->max.y - AABB->min.y / 2));
	}

	void CalculateMass()
	{
		mass = (AABB->max.x - AABB->min.x) * (AABB->max.y - AABB->min.y);
	}

	void CalculateMomentOfInertia()
	{
		float h = (AABB->max.y - AABB->min.y);
		float w = (AABB->max.x - AABB->min.x);

		momentOfInertia = (mass * (h * h + w * w)) / 12;
	}

	void CalculateAABB()
	{
		DynArray<Rectf> l;
		for (int i = 0; i < fixtures.Count(); i++)
		{
			l.PushBack(*(*fixtures.At(i))->AABB);
		}

		delete AABB;

		AABB = Rect<float>::JoinRectangles(l);
		LOG("HELLO");
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
	}

	float mass = 1;
	float momentOfInertia = 1;
	Vec2f forces;
	float torque;
	Vec2f position;
	Vec2f velocity;
	float angle = 0;
	float angularVelocity = 0;
	Vec2f centerOfMass;

	Rectf* AABB;

	DynArray<Shape*> fixtures;

private:

};