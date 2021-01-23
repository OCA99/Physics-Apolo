#include "Planets.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Render.h"

#include "Rigidbody.h"
#include "World.h"

Planet::Planet(Vec2f pos, int radius, int points, float density, const char* path)
{
    p = new Rigidbody(pos * app->scene->world->scale, density, app->scene->world->scale);

    r = radius;
    pts = points;

    img = app->tex->Load(path);

    DynArray<Vec2f>* c = new DynArray<Vec2f>();

    for (float i = 0; i < 2 * M_PI; i += (2 * M_PI) / 120.0f)
    {
        float x = cos(i) * r;
        float y = sin(i) * r;
        c->PushBack(Vec2f(x, y) * app->scene->world->scale);

    }


    Polygon* cp = new Polygon(c);
    p->AddFixture(cp);
    app->scene->world->AddBody(p);

    p->Rotate(1);


}

Planet::~Planet()
{

}

bool Planet::Start()
{
    


    return true;
}