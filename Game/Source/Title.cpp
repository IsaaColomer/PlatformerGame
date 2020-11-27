#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "Intro.h"
#include "Scene.h"
#include "Scene2.h"
#include "ModuleFadeToBlack.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Title::Title() : Module()
{
	name.Create("title");
}

Title::~Title()
{

}

bool Title::Awake()
{
	LOG("Loading Title");
	bool ret = true;

	return ret;
}

// Load assets
bool Title::Start()
{
	app->titleScreen->active = true;

	spaced = false;

	bool ret = true;

	LOG("Loading intro assets");

	titlescreen = app->tex->Load("Assets/Logo/titlescreen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool Title::PreUpdate()
{
	return true;
}

bool Title::Update(float dt)
{


	return true;
}

// Update: draw background
bool Title::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && spaced == false)
	{
		app->fade->Fade(this, (Module*)app->intro, 60);
		spaced = true;
		return true;
	}
	// Draw everything --------------------------------------
	app->render->DrawTexture(titlescreen, 0, 0, NULL);

	return true;
}

bool Title::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing intro");

	app->titleScreen->active = false;
	app->tex->UnLoad(titlescreen);

	return true;
}