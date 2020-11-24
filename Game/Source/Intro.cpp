#include "Intro.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "ModuleFadeToBlack.h"
#include "Log.h"
#include "Defs.h"

Intro::Intro() : Module()
{

}

Intro::~Intro()
{

}

// Load assets
bool Intro::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	introscreen = app->tex->Load("Assets/Textures/introscreen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool Intro::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 90);
	}

	return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	app->render->DrawTexture(introscreen, 0, 0, NULL);

	return ret;
}