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
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

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

	a.PushBack(Vec2f(0, 3));
	a.PushBack(Vec2f(2, 2));
	a.PushBack(Vec2f(1, 1));
	a.PushBack(Vec2f(2, 1));
	a.PushBack(Vec2f(3, 0));
	a.PushBack(Vec2f(0, 0));
	a.PushBack(Vec2f(3, 3));

	b.PushBack(Vec2f(2, 0));
	b.PushBack(Vec2f(3, 0));
	b.PushBack(Vec2f(3, 1));
	b.PushBack(Vec2f(2, 1));

	Polygon* ap = new Polygon(&a);
	Polygon* bp = new Polygon(&b);

	Polygon* ch = ap->ConvexHull();

	int n = ch->vertices->Count();
	for (int i = 0; i < n; i++)
	{
		LOG("%f, %f\n", ch->vertices->At(i)->x, ch->vertices->At(i)->y);
	}

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

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
