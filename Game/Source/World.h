#pragma once

#include "DynArray.h"
#include "Rigidbody.h"

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
		for (int i = 0; i < bodies.Count(); i++)
		{
			(*bodies.At(i))->Update(dt);
		}
	}

	DynArray<Rigidbody*> bodies;
	int scale = 1;
private:
};