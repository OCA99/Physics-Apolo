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
	world = new World();
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
	DynArray<Vec2f> a;
	DynArray<Vec2f> b;

	a.PushBack(Vec2f(0, 0));
	a.PushBack(Vec2f(1, 0));
	a.PushBack(Vec2f(1, 1));
	a.PushBack(Vec2f(0, 1));

	b.PushBack(Vec2f(0, 1));
	b.PushBack(Vec2f(1, 2));
	b.PushBack(Vec2f(1, 1));

	Polygon* ap = new Polygon(&a);
	Polygon* bp = new Polygon(&b);

	Rigidbody* r = new Rigidbody(Vec2f(10.0f, 10.0f));
	r->AddFixture(ap);
	r->AddFixture(bp);
	world->AddBody(r);

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
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
