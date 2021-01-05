#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"
#include "GuiButton.h"

class GuiControl;

struct SDL_Texture;

class Intro : public Module
{
public:
	//Constructor
	Intro();

	//Destructor
	~Intro();

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

	bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	bool fulled;
	bool exit;
	bool vsyncClicked;
	SDL_HintPriority vsync;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* introscreen = nullptr;
	SDL_Texture* configTexture = nullptr;
	SDL_Texture* tutorialScreen = nullptr;
	SDL_Texture* playTexture = nullptr;
	SDL_Texture* exitTexture = nullptr;

	bool tutorial;

	bool gamePaused;

	bool spaced;

	bool checkedFullScreen;
private:
	GuiButton* btnStart;
	GuiButton* btnLoad;
	GuiButton* btnExit;
	GuiButton* btnConfig;
	GuiButton* btnCredits;
	GuiButton* btnTutorial;
};

#endif