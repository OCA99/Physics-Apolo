#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"

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
    return true;
}

bool Player::PreUpdate()
{
    return true;
}

bool Player::Update(float dt)
{
    return true;
}

bool Player::PostUpdate()
{
    SDL_Rect rec = { 0,0,13,16 };
    app->render->DrawTexture(img, 300, 300, &rec);

    return true;
}

bool Player::CleanUp()
{
    return true;
}
