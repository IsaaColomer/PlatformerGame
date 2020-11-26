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
#include "Scene.h"
#include "ModuleFadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Intro::Intro() : Module()
{
	name.Create("intro");
}

Intro::~Intro()
{

}

bool Intro::Awake()
{
	LOG("Loading Intro");
	bool ret = true;

	return ret;
}

// Load assets
bool Intro::Start()
{
	app->intro->active = true;

	bool ret = true;

	LOG("Loading intro assets");

	introscreen = app->tex->Load("Assets/Textures/introscreen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool Intro::PreUpdate()
{
	return true;
}

bool Intro::Update(float dt)
{


	return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 60);
		return true;
	}
	// Draw everything --------------------------------------
	app->render->DrawTexture(introscreen, 0, 0, NULL);

	return true;
}

bool Intro::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing intro");

	app->intro->active = false;
	app->tex->UnLoad(introscreen);

	return true;
}