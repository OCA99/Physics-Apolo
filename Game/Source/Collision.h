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
		if (points != nullptr)
			delete points;
	}

	DynArray<Vec2f>* points = nullptr;
};