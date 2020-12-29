#ifndef __CONFIG_SCENE__
#define __CONFIG_SCENE__

#include "Module.h"
#include "Animation.h"
#include "GuiSlider.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class ConfigScene : public Module
{
public:
	//Constructor
	ConfigScene();

	//Destructor
	~ConfigScene();

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
	SDL_Texture* configscreen = nullptr;

	bool neutral = true;

	bool spaced;

	GuiButton* btnExit;
	GuiCheckBox* btnConfig;
	GuiCheckBox* btnVsync;
	GuiSlider* fxSlider;
	SDL_Rect fxSliderBack;

private:

};
#endif