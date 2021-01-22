#pragma once

#include <math.h>

struct Line {
	double slope;
	double y;

	double evaluate(double x)
	{
		return slope * x + y;
	}
};