#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"

#include "Defs.h"
#include "Vector2.h"
#include "DynArray.h"
#include "Polygon.h"
#include "Rigidbody.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
	world = new World(100);
}

// Destructor
Scene::~Scene()
{
	delete world;
}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	DynArray<Vec2f>* a = new DynArray<Vec2f>();
	DynArray<Vec2f>* b = new DynArray<Vec2f>();

	a->PushBack(Vec2f(0, 0) * world->scale);
	a->PushBack(Vec2f(1, 0) * world->scale);
	a->PushBack(Vec2f(1, 1) * world->scale);
	a->PushBack(Vec2f(0, 1) * world->scale);

	b->PushBack(Vec2f(0, 1) * world->scale);
	b->PushBack(Vec2f(1, 2) * world->scale);
	b->PushBack(Vec2f(1, 1) * world->scale);

	Polygon* ap = new Polygon(a);
	Polygon* bp = new Polygon(b);

	r = new Rigidbody(Vec2f(2.0f, 2.0f) * world->scale, 1, world->scale);
	r->AddFixture(ap);
	r->AddFixture(bp);
	world->AddBody(r);

	DynArray<Vec2f>* c = new DynArray<Vec2f>();
	c->PushBack(Vec2f(0, 0) * world->scale);
	c->PushBack(Vec2f(1, 0) * world->scale);
	c->PushBack(Vec2f(1, 1) * world->scale);
	c->PushBack(Vec2f(0, 1) * world->scale);

	Polygon* cp = new Polygon(c);

	r2 = new Rigidbody(Vec2f(8.0f, 2.0f) * world->scale, 1, world->scale);
	r2->AddFixture(cp);
	world->AddBody(r2);

	r->AddTorque(Vec2f(0, 0) * world->scale, Vec2f(0, 1));
	r2->AddForceOnPoint(Vec2f(1, 0.5f) * world->scale, Vec2f(-1, 0));

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	world->Step(dt);
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	for (int i = 0; i < world->bodies.Count(); i++)
	{
		Rigidbody* r = *world->bodies.At(i);

		for (int j = 0; j < r->fixtures.Count(); j++)
		{
			Shape* s = *r->fixtures.At(j);
			if (s->type == Shape::Type::POLYGON)
			{
				Polygon* p = (Polygon*)s;
				for (int k = 0; k < p->vertices->Count(); k++)
				{
					int l = (k + 1) % p->vertices->Count();
					Vec2f a = *p->vertices->At(k);
					Vec2f b = *p->vertices->At(l);

					app->render->DrawLine(a.x, a.y, b.x, b.y, 255, 0, 0, 255, false);
				}
			}
		}

		app->render->DrawRectangle(SDL_Rect({ (int)r->AABB->min.x, (int)r->AABB->min.y, (int)r->AABB->max.x - (int)r->AABB->min.x, (int)r->AABB->max.y - (int)r->AABB->min.y }), 0, 255, 0, 255, false);
		app->render->DrawRectangle(SDL_Rect({ (int)r->centerOfMass.x - 10, (int)r->centerOfMass.y - 10, 20, 20 }), 0, 255, 0, 255, false);
	}


	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
