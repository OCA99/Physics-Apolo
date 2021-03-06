#pragma once

#include "DynArray.h"
#include "Rigidbody.h"
#include "Pair.h"
#include "Collision.h"

#define GVAR 0.0005
#define DRAGC 0.25
#define DENSITY 1.225
#define K 50


class World
{
public:
	World(int _scale)
	{
		scale = _scale;
	}

	~World()
	{

	}

	void AddBody(Rigidbody* body)
	{
		bodies.PushBack(body);
	}

	void ElasticForce()
	{
		for (int i = 0; i < pairs.Count(); i++)
		{
			Pair<Rigidbody*, Rigidbody*>* a = *pairs.At(i);
			float force = K * (a->first->centerOfMass.DistanceTo(a->second->centerOfMass) / scale);
			Vec2f ab = a->first->centerOfMass - a->second->centerOfMass;
			ab /= ab.Length();
			ab *= force;
			a->second->AddForce((ab * a->second->mass));
			a->first->AddForce((Vec2f(-ab.x, -ab.y) * a->first->mass));
		}
	}

	void Step(float dt)
	{
		for (int i = 0; i < bodies.Count(); i++)
		{
			Rigidbody* r = *bodies.At(i);
			if (r->dead)
				return;
		}
		ElasticForce();
		CalculateGravity();
		UpdateBodies(dt);
		DynArray<Collision*> collisions;
		FindCollisions(collisions);
		SolveCollisions(collisions, dt);
	}

	Vec2f DragForce(Rigidbody* body)
	{

		float velModule = body->velocity.Length();
		float force = 0.5 * (DENSITY * velModule * DRAGC * 0.1);

		Vec2f aDrag = (body->velocity / body->velocity.Length()) * (-force);

		return aDrag;
	}

	DynArray<Rigidbody*> bodies;
	int scale = 1;

	void CreatePair(Rigidbody* a, Rigidbody* b)
	{
		Pair<Rigidbody*, Rigidbody*>* p = new Pair<Rigidbody*, Rigidbody*>(a, b);
		pairs.PushBack(p);
	}


	DynArray<Pair<Rigidbody*,Rigidbody*>*> pairs;

private:

	void UpdateBodies(float dt)
	{
		for (int i = 0; i < bodies.Count(); i++)
		{
			(*bodies.At(i))->Update(dt);
		}
	}

	void FindCollisions(DynArray<Collision*>& collisions)
	{
		DynArray<Collision*> broad;
		BroadPhase(broad);
		NarrowPhase(broad, collisions);
	}

	void BroadPhase(DynArray<Collision*>& broad)
	{
		for (int i = 0; i < bodies.Count(); i++)
		{
			for (int j = i + 1; j < bodies.Count(); j++)
			{

				Rigidbody* a = *bodies.At(i);
				Rigidbody* b = *bodies.At(j);

				if (a->AABB->Intersects(b->AABB))
				{
					Collision* c = new Collision(a, b);
					broad.PushBack(c);
				}
			}
		}
	}

	void NarrowPhase(DynArray<Collision*>& broad, DynArray<Collision*>& collisions)
	{
		for (int i = 0; i < broad.Count(); i++)
		{
			Collision* c = *broad.At(i);
			DynArray<Vec2f>* points = new DynArray<Vec2f>();
			if (c->first->Intersects(c->second, *points))
			{
				Collision* n = new Collision(c->first, c->second);
				n->points = points;
				collisions.PushBack(n);
			}
			delete c;
		}
	}

	void SolveCollisions(DynArray<Collision*>& collisions, float dt)
	{
		for (int i = 0; i < collisions.Count(); i++)
		{
			Collision* c = *collisions.At(i);

			Rigidbody* a = c->first;
			Rigidbody* b = c->second;

			Vec2f plane = Vec2f::fitPoints(c->points) * scale;
			Vec2f perpendicular = Vec2f(plane.y, -plane.x);
		
			Vec2f an = Vec2f(-perpendicular.x, -perpendicular.y);
			Vec2f bn = perpendicular;

			an = an / an.Length();
			bn = bn / bn.Length();

			DynArray<Vec2f> tmp;

			if (a->mass < b->mass)
			{
				Vec2f c = a->centerOfMass - b->centerOfMass;
				c = c / c.Length();

				while (a->Intersects(b, tmp))
				{
					a->Translate(c);
					tmp.Clear();
				}
			}
			else
			{
				Vec2f c = b->centerOfMass - a->centerOfMass;
				c = c / c.Length();

				while (b->Intersects(a, tmp))
				{
					b->Translate(c);
					tmp.Clear();
				}
			}

			Vec2f refB = b->velocity - bn * 2 * b->velocity.dot(bn);
			Vec2f refA = a->velocity - an * 2 * a->velocity.dot(an);

			Vec2f cp = Vec2f::average(c->points);

			double momentA = a->getMoment(cp, b);
			double momentB = b->getMoment(cp, a);

			double sum = momentA + momentB;

			double propA = momentA / sum;
			double propB = momentB / sum;

			float moment;

			refA = refA / refA.Length();
			refB = refB / refB.Length();

			if (a->mass < b->mass)
			{
				moment = a->getMoment(cp);
				a->velocity = refA * (b->velocity - a->velocity).Length() * 0.5f;
				a->velocity += b->velocity;
			}
			else
			{
				moment = b->getMoment(cp);
				b->velocity = refB * (a->velocity - b->velocity).Length() * 0.5f;
				b->velocity += a->velocity;
			}

			if (a->type == Rigidbody::Type::PLAYER && b->type != Rigidbody::Type::BOX)
			{
				float vel = (a->velocity - b->velocity).Length() / scale;
				if (vel > 25)
				{
					a->dead = true;
				}
			}

			if (b->type == Rigidbody::Type::PLAYER && a->type != Rigidbody::Type::BOX)
			{
				float vel = (a->velocity - b->velocity).Length() / scale;
				if (vel > 25)
				{
					b->dead = true;
				}
			}

			if (a->type == Rigidbody::Type::MOON || b->type == Rigidbody::Type::MOON)
			{
				if (a->type == Rigidbody::Type::PLAYER)
					a->gotToMoon = true;

				if (b->type == Rigidbody::Type::PLAYER)
					b->gotToMoon = true;
			}

			if (a->type == Rigidbody::Type::EARTH || b->type == Rigidbody::Type::EARTH)
			{
				if (a->type == Rigidbody::Type::PLAYER && a->gotToMoon)
				{
					b->win = true;
					a->gotToMoon = false;
				}

				if (b->type == Rigidbody::Type::PLAYER && b->gotToMoon)
				{
					b->win = true;
					b->gotToMoon = false;
				}
			}

			if (a->type == Rigidbody::Type::PLAYER && b->type == Rigidbody::Type::BOX)
			{
				a->joinBox = true;
			}

			if (b->type == Rigidbody::Type::PLAYER && a->type == Rigidbody::Type::BOX)
			{
				b->joinBox = true;
			}

		}
	}

	void CalculateGravity()
	{
		for (int i = 0; i < bodies.Count(); i++)
		{
			Rigidbody* a = *bodies.At(i);
			for (int j = i + 1; j < bodies.Count(); j++)
			{
				Rigidbody* b = *bodies.At(j);
				
				float distance = a->centerOfMass.DistanceTo(b->centerOfMass)/scale;
				float force = - GVAR * (a->mass * b->mass) / ((distance * distance) * 20);

				Vec2f ab = (a->centerOfMass - b->centerOfMass);
				Vec2f ba = (b->centerOfMass - a->centerOfMass);

				ab = (ab / ab.Length())*force;
				ba = (ba / ba.Length())*force;

				if (distance > b->gravityMin && distance < b->gravityMax)
					a->AddForce(ab * scale);

				if (distance > a->gravityMin && distance < a->gravityMax)
					b->AddForce(ba * scale);

			}
		}
	}

	
};