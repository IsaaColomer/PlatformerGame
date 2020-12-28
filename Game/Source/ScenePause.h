#ifndef __SCENE_PAUSE__
#define __SCENE_PAUSE__

#include "Module.h"
#include "Animation.h"
#include "GuiSlider.h"
#include "GuiButton.h"

struct SDL_Texture;

class ScenePause : public Module
{
public:
	//Constructor
	ScenePause();

	//Destructor
	~ScenePause();

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
	SDL_Texture* scenepause = nullptr;

	bool neutral = true;

	bool spaced;
private:
	GuiButton* btnExit;
	GuiButton* btnResume;
	GuiButton* btnSettings;
};
#endif