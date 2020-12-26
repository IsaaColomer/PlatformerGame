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
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	btnExit = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	fxSlider = new GuiSlider(5, { 1280 / 2 - 300 / 2, 400, 40, 40 }, "FX");
	fxSlider->SetObserver((Scene*)this);

	fxSliderBack = { 470,400,400,40 };

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

	btnExit->Update(app->input, dt);
	fxSlider->Update(app->input, dt);
	return true;
}

// Update: draw background
bool ConfigScene::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(configscreen, 0, 0, NULL);
	app->render->DrawRectangle(fxSliderBack,154,122,130);
	btnExit->Draw(app->render);
	fxSlider->Draw(app->render);
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