#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"

#include "Defs.h"
#include "Vector2.h"
#include "DynArray.h"
#include "Polygon.h"
#include "Rigidbody.h"
#include "Log.h"
#include "Planets.h"

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
	bg = app->tex->Load("Assets/Textures/backgroundBig.png");

	app->audio->PlayMusic("Assets/Audio/Music/theme.ogg", 1.0f);
	

	earth = new Planet(Vec2f(35.0f, 35.0f), 9, 50, 10000, "Assets/Textures/Planets/Terran.png", 9, 80);
	earth->p->stat = true;
	earth->p->type = Rigidbody::Type::EARTH;
	moon = new Planet(Vec2f(60.0f, 35.0f), 3, 50, 10000, "Assets/Textures/Planets/Baren.png", 3, 20);
	moon->p->type = Rigidbody::Type::MOON;
	boxTexture = app->tex->Load("Assets/Textures/box.png");
	box = new Rigidbody(Vec2f(60.0f, 32.0f) * world->scale, 0.0000003f, world->scale, 0.0f, 0.0f);
	box->type = Rigidbody::Type::BOX;
	DynArray<Vec2f>* points = new DynArray<Vec2f>();
	points->PushBack(Vec2f(0, 0) * app->scene->world->scale);
	points->PushBack(Vec2f(0.3, 0) * app->scene->world->scale);
	points->PushBack(Vec2f(0.3, 0.25) * app->scene->world->scale);
	points->PushBack(Vec2f(0, 0.25) * app->scene->world->scale);

	Polygon* p = new Polygon(points);
	box->AddFixture(p);


	world->AddBody(box);

	athmo = app->tex->Load("Assets/Textures/Planets/athmo.png");
	flag = app->tex->Load("Assets/Textures/flag.png");
	
	//float iny = sqrt(GVAR * earth->p->mass / (earth->p->centerOfMass.DistanceTo(moon->p->centerOfMass)/world->scale));
	moon->p->velocity = Vec2f(0, 6000);

	earthAnimation = new Animation();
	earthAnimation->GenerateAnimation(SDL_Rect({ 0, 0, 100, 100 }), 0, 49, 0, 0);
	earthAnimation->speed = 0.1f;

	moonAnimation = new Animation();
	moonAnimation->GenerateAnimation(SDL_Rect({ 0, 0, 100, 100 }), 0, 49, 0, 0);
	moonAnimation->speed = 0.1f;

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
	//Draw BackGround
	SDL_Rect rec = { 0,0,1280,1280 };
	app->render->DrawTexture(bg, -500, -500, &rec, 10);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		moon->p->gotToMoon = true;

	earthAnimation->Update();
	moonAnimation->Update();

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

					if (app->player->debug)
						app->render->DrawLine(a.x, a.y, b.x, b.y, 255, 0, 0, 255, true);
				}
			}
		}

		if (app->player->debug)
		{
			app->render->DrawRectangle(SDL_Rect({ (int)r->AABB->min.x, (int)r->AABB->min.y, (int)r->AABB->max.x - (int)r->AABB->min.x, (int)r->AABB->max.y - (int)r->AABB->min.y }), 0, 255, 0, 255, false, true);
			app->render->DrawRectangle(SDL_Rect({ (int)r->centerOfMass.x - 10, (int)r->centerOfMass.y - 10, 20, 20 }), 0, 255, 0, 255, false, true);
		}


	}

	if (app->player->debug)
	{
		app->render->DrawLine(earth->p->centerOfMass.x, earth->p->centerOfMass.y, moon->p->centerOfMass.x, moon->p->centerOfMass.y, 255, 0, 0, 255);
		app->render->DrawLine(app->player->r->centerOfMass.x, app->player->r->centerOfMass.y, moon->p->centerOfMass.x, moon->p->centerOfMass.y, 0, 0, 255, 255);
	}
	
	app->render->DrawTexture(athmo, earth->p->position.x - (18 * world->scale), earth->p->position.y - (18 * world->scale), &SDL_Rect({ 0, 0, 200, 200 }), 17.8f);
	app->render->DrawTexture(earth->img, earth->p->position.x - (earth->r * world->scale), earth->p->position.y - (earth->r * world->scale), &earthAnimation->GetCurrentFrame(), 17.8f);
	app->render->DrawTexture(moon->img, moon->p->position.x - (moon->r * world->scale), moon->p->position.y - (moon->r * world->scale), &moonAnimation->GetCurrentFrame(), 5.95f);
	if (app->player->joinedBox)
		app->render->DrawLine(app->player->r->centerOfMass.x, app->player->r->centerOfMass.y, box->centerOfMass.x, box->centerOfMass.y, 255, 255, 255, 255);
	app->render->DrawTexture(boxTexture, box->position.x, box->position.y, &SDL_Rect({ 0, 0, 22, 16 }), 1.55f);

	SDL_Rect tmp = { 0, 0, 512, 512 };
	if(moon->p->gotToMoon)
		app->render->DrawTexture(flag, moon->p->position.x - (moon->r * world->scale) + 100, moon->p->position.y - (moon->r * world->scale) + 50, &tmp, 0.2f);

	//app->render->DrawLine(app->player->r->centerOfMass.x, app->player->r->centerOfMass.y, moon->p->centerOfMass.x, moon->p->centerOfMass.y, 0, 0, 255, 255);

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
