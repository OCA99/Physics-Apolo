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
	bg = app->tex->Load("Assets/Textures/background.png");
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

					app->render->DrawLine(a.x, a.y, b.x, b.y, 255, 0, 0, 255, true);
				}
			}
		}

		app->render->DrawRectangle(SDL_Rect({ (int)r->AABB->min.x, (int)r->AABB->min.y, (int)r->AABB->max.x - (int)r->AABB->min.x, (int)r->AABB->max.y - (int)r->AABB->min.y }), 0, 255, 0, 255, false,true);
		app->render->DrawRectangle(SDL_Rect({ (int)r->centerOfMass.x - 10, (int)r->centerOfMass.y - 10, 20, 20 }), 0, 255, 0, 255, false,true);
	}
	//Draw BackGround
	SDL_Rect rec = { 0,0,640,640 };
	app->render->DrawTexture(bg, -500, -500, &rec, 5);
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
