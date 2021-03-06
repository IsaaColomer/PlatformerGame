#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "EnemyGround.h"
#include "Title.h"
#include "EntityManager.h"
#include "ScenePause.h"
#include "FadeToBlack.h"
#include "Scene3.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "Intro.h"

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
	bool flagAlive;
	bool tpToScene2 = false;
	bool scene2OnScreen;
	
	//array of colliders, first coll[0] is ground
	//[0]=y, [1]=x1, [2]=x2

private:
	SDL_Texture* img;
	SDL_Texture* backg;
	SDL_Texture* portal;

	Timer timer;

	SDL_Texture* food;
	Collider* foodCol;
	SDL_Rect foodRect;

	SDL_Texture* flag;
	SDL_Texture* checked;
	Collider* flagCol;
	SDL_Rect flagRect;
};

#endif // __SCENE_H__