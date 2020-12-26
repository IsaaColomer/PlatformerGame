#ifndef __CREDITS_SCENE__
#define __CREDITS_SCENE__

#include "Module.h"
#include "Animation.h"
#include "GuiSlider.h"
#include "GuiButton.h"

struct SDL_Texture;

class CreditsScene : public Module
{
public:
	//Constructor
	CreditsScene();

	//Destructor
	~CreditsScene();

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
	SDL_Texture* creditsScreen = nullptr;

	bool neutral = true;

	bool spaced;

	GuiButton* btnExit;

private:

};
#endif