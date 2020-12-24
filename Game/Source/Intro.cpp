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
#include "FadeToBlack.h"


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

	spaced = false;

	bool ret = true;

	LOG("Loading intro assets");

	introscreen = app->tex->Load("Assets/Screens/Title/intro_screen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
	btnStart->SetObserver((Scene*) this);

	btnExit = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);

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
	btnStart->Update(app->input, dt);
	btnExit->Update(app->input, dt);

	return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && spaced == false)
	{
		app->fade->Fade(this, (Module*)app->scene, 60);
		spaced = true;
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (app->intro->active)
			app->fade->Fade((Module*)app->intro, (Module*)app->scene, 60);
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (app->intro->active)
			app->fade->Fade((Module*)app->intro, (Module*)app->scene2, 60);
		return true;
	}
	// Draw everything --------------------------------------
	app->render->DrawTexture(introscreen, 0, 0, NULL);

	btnStart->Draw(app->render);
	btnExit->Draw(app->render);

	return true;
}

bool Intro::CleanUp()
{
	if (!active)
		return true;
	app->player->CleanUp();
	LOG("Freeing intro");

	app->intro->active = false;
	app->tex->UnLoad(introscreen);

	return true;
}