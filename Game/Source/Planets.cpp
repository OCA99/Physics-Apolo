#include "Planets.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Render.h"

#include "World.h"

Planet::Planet(Vec2f pos, int radius, int points, float density, const char* path, float gmin, float gmax)
{
    p = new Rigidbody(pos * app->scene->world->scale, density, app->scene->world->scale, gmin, gmax);

    r = radius;
    pts = points;

    img = app->tex->Load(path);

    DynArray<Vec2f>* c = new DynArray<Vec2f>();

    for (float i = 0; i < 2 * M_PI; i += (2 * M_PI) / (float)points)
    {
        float x = cos(i) * r;
        float y = sin(i) * r;
        c->PushBack(Vec2f(x, y) * app->scene->world->scale);
    }

    Polygon* cp = new Polygon(c);
    p->AddFixture(cp);
    app->scene->world->AddBody(p);
}

Planet::~Planet()
{

}

bool Planet::Start()
{
    


    return true;
}