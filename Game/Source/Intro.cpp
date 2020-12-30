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
#include "Window.h"
#include "Scene2.h"
#include "Scene3.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "ConfigScene.h"
#include "GuiSlider.h"

#include "Defs.h"
#include "Log.h"
class GuiSlider;
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
	exit = false;
	vsyncClicked = false;
	app->intro->active = true;

	spaced = false;

	gamePaused = false;

	bool ret = true;

	LOG("Loading intro assets");

	introscreen = app->tex->Load("Assets/Screens/Title/intro_screen.png");

	app->audio->PlayMusic("Assets/Audio/Music/music_menu.ogg",0.2f);
	//app->audio->PlayMusic("Assets/audio/Music/music_spy.ogg");

	btnLoad = new GuiButton(6, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "LOAD");
	btnLoad->SetObserver((Scene*)this);
	btnLoad->SetTexture(app->tex->Load("Assets/GUI/load.png"), app->tex->Load("Assets/GUI/load_selected.png"), app->tex->Load("Assets/GUI/load_focused.png"));

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 200, 300, 80 }, "START");
	btnStart->SetObserver((Scene*) this);
	btnStart->SetTexture(app->tex->Load("Assets/GUI/play.png"), app->tex->Load("Assets/GUI/play_selected.png"), app->tex->Load("Assets/GUI/play_focused.png"));

	btnConfig = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "CONFIG");
	btnConfig->SetObserver((Scene*)this);
	btnConfig->SetTexture(app->tex->Load("Assets/GUI/settings.png"), app->tex->Load("Assets/GUI/settings_selected.png"), app->tex->Load("Assets/GUI/settings_focused.png"));

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*) this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	btnCredits = new GuiButton(0, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "CREDITS");
	btnCredits->SetObserver((Scene*)this);
	btnCredits->SetTexture(app->tex->Load("Assets/GUI/credits.png"), app->tex->Load("Assets/GUI/credits_selected.png"), app->tex->Load("Assets/GUI/credits_focused.png"));

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	fulled = false;

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
	bool ret = true;
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

	if (exit == true)
	{
		return false;
	}

	// Draw everything --------------------------------------
	app->render->DrawTexture(introscreen, 0, 0, NULL);

	btnLoad->Draw(app->render);
	btnStart->Draw(app->render);
	btnConfig->Draw(app->render);
	btnExit->Draw(app->render);
	btnCredits->Draw(app->render);

	return ret;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fade->Fade((Module*)app->intro, (Module*)app->scene, 10);
		if (control->id == 0) app->fade->Fade((Module*)app->intro, (Module*)app->creditsscene, 10);
		if (control->id == 2) app->fade->Fade((Module*)this, (Module*)app->configscene, 10);
		if (control->id == 3) app->fade->Fade((Module*)this, (Module*)app->intro, 1);
		if (control->id == 4)
		{
			app->intro->exit = true;
		}
		if (control->id == 5)
		{
			
		}
		if (control->id == 6)
		{
			
		}
		if (control->id == 10)
		{
			if (app->intro->gamePaused == false)
			{
				app->fade->Fade(this, (Module*)app->intro, 10);
			}
			else
			{
				app->fade->Fade(this, (Module*)app->scenepause, 10);
			}
		}
		if (control->id == 12)
		{
			app->fade->Fade((Module*)this, (Module*)app->intro, 1);
		}
	}

	case GuiControlType::SLIDER:
	{
		if (control->id == 5)
		{
			app->audio->VolumeChanger(100 * (control->bounds.x - app->configscene->fxSliderBack.x) / app->configscene->fxSliderBack.w);
		}
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 8 && app->intro->fulled == false)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			app->intro->fulled = true;
			control->vsyncChecked = true;
		}
		else if (control->id == 8 && app->intro->fulled)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
			app->intro->fulled = false;
			control->vsyncChecked = false;
		}
		if (control->id == 9 && app->intro->vsyncClicked == false)
		{
			/*SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");*/
			printf("vsync = true");
			SDL_GetHint("SDL_RENDERER_PRESENTVSYNC");
			SDL_SetHint("SDL_RENDERER_PRESENTVSYNC", "1");
			app->intro->vsyncClicked = true;
		}
		else if (control->id == 9 && app->intro->vsyncClicked == true)
		{
			SDL_GetHint("SDL_RENDERER_PRESENTVSYNC");
			SDL_SetHint("SDL_RENDERER_PRESENTVSYNC", "0");
			printf("vsync = false");
			app->intro->vsyncClicked = false;
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