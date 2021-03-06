#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Title : public Module
{
public:
	//Constructor
	Title();

	//Destructor
	~Title();

	bool Awake();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	bool PreUpdate();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* titlescreen = nullptr;
	SDL_Texture* youwin = nullptr;
	SDL_Texture* youlose = nullptr;
	bool neutral = true;

	bool spaced;
};

#endif