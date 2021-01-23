#ifndef __PLANET_H__
#define __PLANET_H__

#include "World.h"

struct SDL_Texture;

class Planet 
{
public:

	Planet(Vec2f pos, int radius, int points, float density, const char* path);

	// Destructor
	virtual ~Planet();

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

	Rigidbody* p;

	int pts;
	int r;
private:

	SDL_Texture* img;


	//Properties


};
#endif // __PLANET_H__