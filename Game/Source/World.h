#pragma once

#include "DynArray.h"
#include "Rigidbody.h"
#include "Pair.h"
#include "Collision.h"

#define GVAR 0.0005
#define DRAGC 0.1
#define DENSITY 0

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
		CalculateGravity();
		UpdateBodies(dt);
		DynArray<Collision*> collisions;
		FindCollisions(collisions);
		SolveCollisions(collisions, dt);
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

			//app->render->DrawLine(a->centerOfMass.x, a->centerOfMass.y, a->centerOfMass.x + plane.x, a->centerOfMass.y + plane.y, 255, 0, 0, 255, false);

			//Vec2f perpendicular = a->centerOfMass - b->centerOfMass;
		
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

			/*Vec2f deltaB = b->centerOfMass - a->centerOfMass;

			float dot = bn.x * deltaB.x + bn.y * deltaB.y;

			if (dot < 0)
			{
				bn = Vec2f(-bn.x, -bn.y);
			}


			Vec2f deltaA = a->centerOfMass - b->centerOfMass;

			dot = an.x * deltaA.x + an.y * deltaA.y;

			if (dot < 0)
			{
				an = Vec2f(-an.x, -an.y);
			}*/

			Vec2f refB = b->velocity - bn * 2 * b->velocity.dot(bn);
			Vec2f refA = a->velocity - an * 2 * a->velocity.dot(an);

			//app->render->DrawLine(b->centerOfMass.x, b->centerOfMass.y, b->centerOfMass.x + bn.x * scale, b->centerOfMass.y + bn.y * scale, 0, 255, 0, 255, false);
			/*app->render->DrawLine(b->centerOfMass.x, b->centerOfMass.y, b->centerOfMass.x + refB.x * scale, b->centerOfMass.y + refB.y * scale, 0, 0, 255, 255, false);
			app->render->DrawLine(b->centerOfMass.x, b->centerOfMass.y, b->centerOfMass.x + b->velocity.x * scale, b->centerOfMass.y + b->velocity.y * scale, 255, 0, 0, 255, false);*/
			
			Vec2f cp = Vec2f::average(c->points);
			/*app->render->DrawRectangle(SDL_Rect({ (int)cp.x - 5, (int)cp.y - 5, 10, 10 }), 255, 0, 0, 255, false, false);*/

			//while (b->Intersects(a, tmp))
			//{
			//	if (b->velocity.IsZero())
			//	{
			//		break;
			//	}

			//	b->Translate(Vec2f(-b->velocity.x, -b->velocity.y));
			//	
			//	Vec2f d = (cp - (b->centerOfMass - b->position));
			//	d /= scale;
			//	int t = (d.x * bn.y - d.y * bn.x > 0) ? 1 : -1;

			//	//b->Rotate(t * dt);
			//	tmp.Clear();
			//}

			//tmp.Clear();
			//while (a->Intersects(b, tmp))
			//{

			//	if (a->velocity.IsZero())
			//	{
			//		break;
			//	}
			//	a->Translate(Vec2f(-a->velocity.x, -a->velocity.y));

			//	Vec2f d = (cp - (a->centerOfMass - a->position));
			//	d /= scale;
			//	int t = (d.x * an.y - d.y * an.x > 0) ? 1 : -1;

			//	//a->Rotate(t * dt);

			//	tmp.Clear();
			//}

			double momentA = a->getMoment(cp, b);
			double momentB = b->getMoment(cp, a);

			// RELATIVE MOMENT LUL
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
				//a->AddForceOnPoint(cp, (an * moment));
			}
			else
			{
				moment = b->getMoment(cp);
				b->velocity = refB * (a->velocity - b->velocity).Length() * 0.5f;
				b->velocity += a->velocity;
				//b->AddForceOnPoint(cp, (bn * moment));
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

	void DragForce()
	{
		for (int i = 0; i < bodies.Count(); i++)
		{
			Rigidbody* a = *bodies.At(i);

			float velModule = sqrt((a->velocity.x * a->velocity.x) + (a->velocity.y + a->velocity.y));
			float force = 0.5*(DENSITY * velModule * DRAGC * 1);

			Vec2f aDrag = (a->velocity / a->velocity.Length())* (-force);

			a->AddForce(aDrag);

		}
	}
};