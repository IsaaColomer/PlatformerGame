#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"


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

public:
	bool foodAlive;
	bool flagAlive;

	//array of colliders, first coll[0] is ground
	//[0]=y, [1]=x1, [2]=x2

private:
	SDL_Texture* img;
	SDL_Texture* backg;
	SDL_Texture* portal;

	SDL_Texture* food;
	Collider* foodCol;
	SDL_Rect foodRect;

	SDL_Texture* flag;
	Collider* flagCol;
	SDL_Rect flagRect;
};

#endif // __SCENE_H__