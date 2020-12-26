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

	btnLoad = new GuiButton(0, { 1280 / 2 - 300 / 2, 200, 300, 80 }, "LOAD");
	btnLoad->SetObserver((Scene*)this);
	btnLoad->SetTexture(app->tex->Load("Assets/GUI/load.png"), app->tex->Load("Assets/GUI/load_selected.png"), app->tex->Load("Assets/GUI/load_focused.png"));

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
	btnStart->SetObserver((Scene*) this);
	btnStart->SetTexture(app->tex->Load("Assets/GUI/play.png"), app->tex->Load("Assets/GUI/play_selected.png"), app->tex->Load("Assets/GUI/play_focused.png"));

	btnConfig = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "CONFIG");
	btnConfig->SetObserver((Scene*)this);
	btnConfig->SetTexture(app->tex->Load("Assets/GUI/settings.png"), app->tex->Load("Assets/GUI/settings_selected.png"), app->tex->Load("Assets/GUI/settings_focused.png"));

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*) this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	btnCredits = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "CREDITS");
	btnCredits->SetObserver((Scene*)this);
	btnCredits->SetTexture(app->tex->Load("Assets/GUI/credits.png"), app->tex->Load("Assets/GUI/credits_selected.png"), app->tex->Load("Assets/GUI/credits_focused.png"));

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
	btnLoad->Update(app->input, dt);
	btnStart->Update(app->input, dt);
	btnConfig->Update(app->input, dt);
	btnExit->Update(app->input, dt);
	btnCredits->Update(app->input, dt);

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

	btnLoad->Draw(app->render);
	btnStart->Draw(app->render);
	btnConfig->Draw(app->render);
	btnExit->Draw(app->render);
	btnCredits->Draw(app->render);
	return true;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fade->Fade((Module*)app->intro, (Module*)app->scene, 10);
		else if (control->id == 2) app->fade->Fade((Module*)app->intro, (Module*)app->configscene, 1);
		else if (control->id == 3) app->fade->Fade((Module*)app->configscene, (Module*)app->intro, 1);
		else if (control->id == 4) 
			SDL_Quit;
	}

	case GuiControlType::SLIDER:
	{
		if (control->id == 5)
		{

		}
	}
	default: break;
	}

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