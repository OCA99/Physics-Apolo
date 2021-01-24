#include "Player.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Scene.h"
#include "Input.h"

#include "Polygon.h"
#include <math.h>
#include "Log.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Awake(pugi::xml_node&)
{
    bool ret = true;

    return ret;
}

bool Player::Start()
{
    img = app->tex->Load("Assets/Textures/player.png");
    arrow = app->tex->Load("Assets/Textures/arrow.png");

    //BODY
    r = new Rigidbody(Vec2f(44.0f, 35.0f) * app->scene->world->scale, 0.000001f, app->scene->world->scale, 0, 0);
    r->type = Rigidbody::Type::PLAYER;
    DynArray<Vec2f>* c = new DynArray<Vec2f>();
    c->PushBack(Vec2f(0.2f, 0.1) * app->scene->world->scale);
    c->PushBack(Vec2f(0.4f, 0.6f) * app->scene->world->scale);
    c->PushBack(Vec2f(0, 0.6f) * app->scene->world->scale);

    Polygon* cp = new Polygon(c);
    r->AddFixture(cp);
    app->scene->world->AddBody(r);

    r->Rotate(M_PI / 2);

    return true;
}

bool Player::PreUpdate()
{

    return true;
}

bool Player::Update(float dt)
{
    rec = { 17,0,13,26 };
    if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
    {
        Vec2f dir = Vec2f(0, -0.00004f);
        dir = Vec2f(dir.x * cos(r->angle) - dir.y * sin(r->angle), dir.x * sin(r->angle) + dir.y * cos(r->angle));
        r->AddForce(dir);
        rec = { 0,0,13,26 };
    }
    if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        r->Rotate(0.1f);
        rec = { 0,0,13,26 };


    }

    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        r->Rotate(-0.1f);
        rec = { 0,0,13,26 };

    }
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
    {
        debug = !debug;
    }

    if (r->centerOfMass.DistanceTo(app->scene->earth->p->centerOfMass)/app->scene->world->scale < app->scene->earth->r + 6.0f)
    {
        drag = app->scene->world->DragForce(r);
        Vec2f f = drag * r->mass;
        r->AddForce(f);
    }

    return true;
}

bool Player::PostUpdate()
{

    float ang = r->angle * 180 / M_PI;

    app->render->DrawTexture(img, r->position.x, r->position.y, &rec, 3, 1.0f, ang);



    float angle = 0;

    if (r->gotToMoon == false)
    {
        angle = r->centerOfMass.Angle(app->scene->moon->p->centerOfMass);
    }
    else
    {
        angle = r->centerOfMass.Angle(app->scene->earth->p->centerOfMass);
    }

    angle += M_PI / 2;

    app->render->DrawTexture(arrow, 0 - app->render->camera.x + app->render->camera.w / 2 - 31, 0 - app->render->camera.y + app->render->camera.h - 85, &SDL_Rect({ 0, 0, 62, 71 }), 1, 1, angle * (180 / M_PI));

    if (r->centerOfMass.DistanceTo(app->scene->earth->p->centerOfMass) / app->scene->world->scale < app->scene->earth->r + 6.0f && debug)
    {
        app->render->DrawLine(r->centerOfMass.x, r->centerOfMass.y, app->scene->earth->p->centerOfMass.x, app->scene->earth->p->centerOfMass.y, 255, 0, 0, 255);
        app->render->DrawLine(r->centerOfMass.x, r->centerOfMass.y, r->centerOfMass.x + drag.x, r->centerOfMass.y + drag.y, 0, 255, 0, 255);
    }

    return true;
}

bool Player::CleanUp()
{
    return true;
}


