#pragma once

#include "DynArray.h"
#include "Rigidbody.h"
#include "Pair.h"
#include "Collision.h"

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

	void Step(float dt)
	{
		UpdateBodies(dt);
		DynArray<Collision*> collisions;
		FindCollisions(collisions);
	}

	DynArray<Rigidbody*> bodies;
	int scale = 1;
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
			for (int j = 0; j < bodies.Count(); j++)
			{
				if (i == j)
					continue;

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

	void SolveCollisions(DynArray<Collision*>& collisions)
	{
		for (int i = 0; i < collisions.Count(); i++)
		{
			Collision* c = *collisions.At(i);
		}
	}
};