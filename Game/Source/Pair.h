#pragma once

template <class A, class B>
struct Pair
{
	Pair(A f, B s)
	{
		first = f;
		second = s;
	}

	A first;
	B second;
};