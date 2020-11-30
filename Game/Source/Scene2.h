#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

class Scene2 : public Module
{
public:

	Scene2();

	// Destructor
	virtual ~Scene2();

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

public:
	bool foodAlive;
	
	//array of colliders, first coll[0] is ground
	//[0]=y, [1]=x1, [2]=x2

private:
	SDL_Texture* img;
	SDL_Texture* backg;
	SDL_Texture* portal;
	SDL_Texture* food;
	Collider* foodCol;
	SDL_Rect foodRect;
};

#endif // __SCENE_H__