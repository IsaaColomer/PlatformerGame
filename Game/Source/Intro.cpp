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
#include "Font.h"

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

	config = false;

	spaced = false;

	gamePaused = false;

	bool ret = true;

	LOG("Loading intro assets");
	
	introscreen = app->tex->Load("Assets/Screens/Title/intro_screen.png");
	tutorialScreen = app->tex->Load("Assets/Screens/Title/tutorial_screen.png");

	app->audio->PlayMusic("Assets/Audio/Music/music_menu.ogg",0.2f);
	//app->audio->PlayMusic("Assets/audio/Music/music_spy.ogg");

	btnLoad = new GuiButton(6, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "LOAD");
	btnLoad->SetObserver((Scene*)this);
	btnLoad->SetDisabledTexture(app->tex->Load("Assets/GUI/selected_loadoff.png"));

	if (app->savedFile == false)
	{
		btnLoad->state = GuiControlState::DISABLED;
	}
	btnLoad->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected.png"), app->tex->Load("Assets/GUI/selected_focused.png"));

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 200, 300, 80 }, "START");
	btnStart->SetObserver((Scene*) this);
	btnStart->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected.png"), app->tex->Load("Assets/GUI/selected_focused.png"));

	btnConfig = new GuiButton(2, { 1280 / 2 - 415 / 2, 400, 300, 80 }, "CONFIG");
	btnConfig->SetObserver((Scene*)this);
	btnConfig->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected_long.png"), app->tex->Load("Assets/GUI/selected_longf.png"));

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*) this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected.png"), app->tex->Load("Assets/GUI/selected_focused.png"));

	btnCredits = new GuiButton(0, { (1280 / 2 - 300 / 2)+450, 600, 300, 80 }, "CREDITS");
	btnCredits->SetObserver((Scene*)this);
	btnCredits->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected_long.png"), app->tex->Load("Assets/GUI/selected_longf.png"));

	btnTutorial = new GuiButton(16, { (1280 / 2 - 290 / 2) - 450, 625, 300, 80 }, "TUTORIAL");
	btnTutorial->SetObserver((Scene*)this);
	btnTutorial->SetTexture(app->tex->Load("Assets/GUI/blanck.png"), app->tex->Load("Assets/GUI/selected_long.png"), app->tex->Load("Assets/GUI/selected_longf.png"));

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
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->intro->debug = !app->intro->debug;
	}
	
	if (!tutorial && !config)
	{
		btnTutorial->Update(app->input, dt);
		btnLoad->Update(app->input, dt);
		btnStart->Update(app->input, dt);
		btnConfig->Update(app->input, dt);
		btnExit->Update(app->input, dt);
		btnCredits->Update(app->input, dt);
	}
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
	else if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		tutorial = false;
	}

	if (exit == true)
	{
		return false;
	}
	if (tutorial)
	{
		app->render->DrawTexture(tutorialScreen, 0, 0, NULL);
	}
	else
	{
		// Draw everything --------------------------------------
		app->render->DrawTexture(introscreen, 0, 0, NULL);

		btnLoad->Draw(app->render);

		sprintf_s(text, 64, "Load", 56);
		app->render->DrawText(app->render->font, text, 521, 285, 50, 5, { 255, 255, 43, 255 });

		btnStart->Draw(app->render);

		sprintf_s(text, 64, "Play", 56);
		app->render->DrawText(app->render->font, text, 521, 185, 50, 5, { 255, 255, 43, 255 });

		btnConfig->Draw(app->render);

		sprintf_s(text, 64, "Settings", 56);
		app->render->DrawText(app->render->font, text, 480, 390, 50, 5, { 255, 255, 43, 255 });

		btnExit->Draw(app->render);

		sprintf_s(text, 64, "Exit", 56);
		app->render->DrawText(app->render->font, text, 530, 485, 50, 5, { 255, 255, 43, 255 });

		btnCredits->Draw(app->render);

		sprintf_s(text, 64, "Credits", 56);
		app->render->DrawText(app->render->font, text, (1280 / 2 - 300 / 2) + 500, 590, 50, 5, { 255, 255, 43, 255 });

		btnTutorial->Draw(app->render);

		sprintf_s(text, 64, "Tutorial", 56);
		app->render->DrawText(app->render->font, text, (1280 / 2 - 220 / 2) - 450, 610, 50, 5, { 255, 255, 43, 255 });

	}

	return ret;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{

		if (control->id == 1)
		{
			app->escaped = false;
			app->fade->Fade((Module*)app->intro, (Module*)app->scene, 10);
		}
		if (control->id == 0)
		{
			app->escaped = false;
			app->fade->Fade((Module*)app->intro, (Module*)app->creditsscene, 10);
		}
		if (control->id == 2)
		{
			app->intro->config = true;
			app->escaped = false;
			app->fade->Fade((Module*)this, (Module*)app->configscene, 10);
			if(app->scene->sceneOnScreen)
				app->fade->Fade((Module*)app->scene, (Module*)app->configscene, 10);
		}
		if (control->id == 3)
		{
			app->escaped = false;
			app->fade->Fade((Module*)this, (Module*)app->intro, 1);
		}
		if (control->id == 4)
		{
			app->intro->exit = true;
		}
		if (control->id == 5)
		{
			app->escaped = false;
		}
		if (control->id == 6)
		{
			app->escaped = false;
			app->LoadGame();
		}
		if (control->id == 10)
		{
			app->intro->config = false;
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
			app->player->coinsCollected = 0;
			app->fade->Fade((Module*)this, (Module*)app->intro, 1);
			app->scene->CleanUp();
			app->scene2->CleanUp();
			app->scene3->CleanUp();
		}

		if (control->id == 13)
		{		
			app->scene->tpToScene = true;
			app->scene2->tpToScene2 = false;
			app->escaped = false;
			if (app->scene->tpToScene)
			{
				app->player->cp.x = 900;
				app->player->cp.y = 170;
			}
			if (app->scene2->scene2OnScreen)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 1);
			}
			if (app->scene3->scene3OnScreen)
			{
				app->fade->Fade((Module*)app->scene3, (Module*)app->scene, 1);
			}
		}
		if (control->id == 14)
		{
			app->scene2->tpToScene2 = true;
			app->scene->tpToScene = false;

			app->escaped = false;
			if (app->scene->sceneOnScreen)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 1);
			}
			if (app->scene2->scene2OnScreen)
			{
				app->player->cp.x = 1150;
				app->player->cp.y = 170;
			}
			if (app->scene3->scene3OnScreen)
			{
				app->fade->Fade((Module*)app->scene3, (Module*)app->scene2, 1);
			}
				
		}
		if (control->id == 16)
		{
			app->intro->tutorial = !app->intro->tutorial;
		}
	}

	case GuiControlType::SLIDER:
	{
		if (control->id == 5)
		{
			if(app->configscene->musicSliderBack.w > 0)
				app->audio->MusicVolumeChanger(100 * (control->bounds.x - app->configscene->musicSliderBack.x) / app->configscene->musicSliderBack.w);
		}
		if (control->id == 7)
		{
			if (app->configscene->fxSliderBack.w > 0)
				app->audio->FxVolumeChanger(100 * (control->bounds.x - app->configscene->fxSliderBack.x) / app->configscene->fxSliderBack.w);
		}
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 8 && app->intro->checkedFullScreen == false)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			app->intro->checkedFullScreen = true;
		}
		else if(control->id== 8 && app->intro->checkedFullScreen)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
		}
		if (control->id == 9 && app->intro->vsyncClicked == false)
		{
			/*SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");*/
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