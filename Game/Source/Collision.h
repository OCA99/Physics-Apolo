#pragma once

#include "Pair.h"
#include "Rigidbody.h"

struct Collision : Pair<Rigidbody*, Rigidbody*>
{
public:
	Collision(Rigidbody* a, Rigidbody* b) : Pair(a, b)
	{}

	~Collision()
	{
		delete points;
	}

	DynArray<Vec2f>* points;
};