#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#define grav -0.001f

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

	float cpx, cpy, vcy;
	float coll[11][3] =
	{
		//700,0,2520,
		560,280,490,
		420,560,630
	};
private:
	SDL_Texture* img;
	SDL_Texture* character;
	SDL_Texture* backg;
};

#endif // __SCENE_H__