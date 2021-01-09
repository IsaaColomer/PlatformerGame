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
#include "ConfigScene.h"
#include "Title.h"
#include "GuiControl.h"

#include "Defs.h"
#include "Log.h"

ConfigScene::ConfigScene() : Module()
{
	name.Create("config");
}

ConfigScene::~ConfigScene()
{

}

bool ConfigScene::Awake()
{
	LOG("Loading Title");
	bool ret = true;

	return ret;
}

// Load assets
bool ConfigScene::Start()
{
	app->configscene->active = true;

	spaced = false;

	bool ret = true;

	LOG("Loading config assets");

	configscreen = app->tex->Load("Assets/Screens/Title/config_screen.png");

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	btnBack = new GuiButton(10, { 60, 29, 78, 23 }, "BACK");
	btnBack->SetObserver((Scene*)this);
	btnBack->SetTexture(app->tex->Load("Assets/GUI/back.png"), app->tex->Load("Assets/GUI/back_selected.png"), app->tex->Load("Assets/GUI/back_focused.png"));

	musicSlider = new GuiSlider(5, { 1280 / 2 , 400, 40, 40 }, "MUSIC");
	musicSlider->SetObserver((Scene*)this);
	musicSlider->SetTexture(app->tex->Load("Assets/GUI/fx.png"), app->tex->Load("Assets/GUI/fx_selected.png"), app->tex->Load("Assets/GUI/fx_focused.png"));

	fxSlider = new GuiSlider(7, { 1280 / 2 , 500, 40, 40 }, "FX");
	fxSlider->SetObserver((Scene*)this);
	fxSlider->SetTexture(app->tex->Load("Assets/GUI/fx.png"), app->tex->Load("Assets/GUI/fx_selected.png"), app->tex->Load("Assets/GUI/fx_focused.png"));

	btnConfig = new GuiCheckBox(8, {600, 180, 40, 40 }, "FULLSCREEN", app->intro->fulled);
	btnConfig->SetObserver((Scene*)this);
	btnConfig->SetTexture(app->tex->Load("Assets/GUI/fullscreen.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"));

	btnVsync = new GuiCheckBox(9, { 600, 240, 40, 40 }, "VSYNC", app->intro->vsyncClicked);
	btnVsync->SetObserver((Scene*)this);
	btnVsync->SetTexture(app->tex->Load("Assets/GUI/fullscreen.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"));

	musicSliderBack = { 470,400,400,40 };
	fxSliderBack = { 470,500,400,40 };

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool ConfigScene::PreUpdate()
{
	return true;
}

bool ConfigScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->intro->debug = !app->intro->debug;
	}
	btnExit->Update(app->input, dt);
	musicSlider->Update(app->input, dt);
	fxSlider->Update(app->input, dt);
	btnConfig->Update(app->input, dt);
	btnVsync->Update(app->input, dt);
	btnBack->Update(app->input, dt);

	if (app->intro->exit == true)
	{
		return false;
	}

	return true;
}

// Update: draw background
bool ConfigScene::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(configscreen, 0, 0, NULL);
	app->render->DrawRectangle(musicSliderBack,154,122,130);
	app->render->DrawRectangle(fxSliderBack,154,122,130);
	btnExit->Draw(app->render);
	musicSlider->Draw(app->render);
	fxSlider->Draw(app->render);
	btnConfig->Draw(app->render);
	btnBack->Draw(app->render);
	btnVsync->Draw(app->render);

	return true;
}

bool ConfigScene::CleanUp()
{
	if (!active)return true;

	LOG("Freeing config");

	app->configscene->active = false;
	app->tex->UnLoad(configscreen);

	return true;
}