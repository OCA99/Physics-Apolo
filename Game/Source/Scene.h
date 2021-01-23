#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "World.h"
#include "Planets.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	World* world;

	

	Planet* moon;
private:
	Planet* earth;
	SDL_Texture* img;
	SDL_Texture* bg;
	Rigidbody* r;
	Rigidbody* r2;
};

#endif // __SCENE_H__