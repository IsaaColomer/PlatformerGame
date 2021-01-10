#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"

class GuiControl;

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

	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	bool flagAlive;
	bool doStart = true;
	bool mapLoaded;
	bool tpToScene = false;

	bool sceneOnScreen;
private:
	SDL_Texture* img;
	SDL_Texture* backg;
	SDL_Texture* portal;

	Timer timer;

	SDL_Texture* flag;
	SDL_Texture* checked;
	Collider* flagCol;
	SDL_Rect flagRect;
};

#endif // __SCENE_H__