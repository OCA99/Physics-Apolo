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

    //BODY
    r = new Rigidbody(Vec2f(2.0f, 2.0f) * app->scene->world->scale, 1, app->scene->world->scale);

    DynArray<Vec2f>* c = new DynArray<Vec2f>();
    c->PushBack(Vec2f(0.2f, 0) * app->scene->world->scale);
    c->PushBack(Vec2f(0.4f, 0.5f) * app->scene->world->scale);
    c->PushBack(Vec2f(0, 0.5f) * app->scene->world->scale);

    Polygon* cp = new Polygon(c);
    r->AddFixture(cp);
    app->scene->world->AddBody(r);

    r->Rotate(1);

    return true;
}

bool Player::PreUpdate()
{

    return true;
}

bool Player::Update(float dt)
{

    if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
    {
        Vec2f dir = Vec2f(0, -0.1f);
        dir = Vec2f(dir.x * cos(r->angle) - dir.y * sin(r->angle), dir.x * sin(r->angle) + dir.y * cos(r->angle));
        r->AddForce(dir);
    }
    if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        r->Rotate(0.1f);
    }
    if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        r->Rotate(-0.1f);
    }

    return true;
}

bool Player::PostUpdate()
{
    SDL_Rect rec = { 0,0,13,16 };
    float ang = r->angle * 180 / M_PI;
    app->render->DrawTexture(img, r->position.x, r->position.y, &rec, 3, 1.0f, ang);


    return true;
}

bool Player::CleanUp()
{
    return true;
}


