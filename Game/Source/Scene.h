#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#define grav -0.1f

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

	//player/charater x pos, y pos, velocity in y
	float cpx, cpy, vcy;

	//onGround
	bool ong;

	//array of colliders, first coll[0] is ground
	//[0]=y, [1]=x1, [2]=x2
	float coll[11][3] =
	{
		700,0,2520,
		560,280,490,
		420,560,630,
		280,770,980,
		490,980,1260,
		350,1330,1610,
		280,1680,1750,
		350,1890,1960,
		420,2030,2100,
		490,2170,2240,
		420,2310,2450
	};
private:
	SDL_Texture* img;
	SDL_Texture* character;
	SDL_Texture* backg;
};

#endif // __SCENE_H__