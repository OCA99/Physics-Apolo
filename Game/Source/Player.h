#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "World.h"

struct SDL_Texture;

class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

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

	Rigidbody* r;
private:

	SDL_Texture* img;


	//Properties


};
#endif // __PLAYER_H__