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
#include "ScenePause.h"

#include "Defs.h"
#include "Log.h"

ScenePause::ScenePause() : Module()
{
	name.Create("scene pause");
}

ScenePause::~ScenePause()
{

}

bool ScenePause::Awake()
{
	LOG("Loading Title");
	bool ret = true;

	return ret;
}

// Load assets
bool ScenePause::Start()
{
	app->scenepause->active = true;

	spaced = false;

	bool ret = true;

	LOG("Loading config assets");

	configscreen = app->tex->Load("Assets/Screens/Title/config_screen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	btnExit = new GuiButton(10, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	/*fxSlider = new GuiSlider(5, { 1280 / 2 - 300 / 2, 400, 40, 40 }, "FX");
	fxSlider->SetObserver((Scene*)this);
	fxSlider->SetTexture(app->tex->Load("Assets/GUI/fx.png"), app->tex->Load("Assets/GUI/fx_selected.png"), app->tex->Load("Assets/GUI/fx_focused.png"));

	btnConfig = new GuiButton(8, { (1280 / 2 - 300 / 2), 180, 40, 40 }, "VSYNC");
	btnConfig->SetObserver((Scene*)this);
	btnConfig->SetTexture(app->tex->Load("Assets/GUI/fullscreen.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"));

	btnVsync = new GuiButton(9, { (1280 / 2 - 300 / 2), 240, 40, 40 }, "VSYNC");
	btnVsync->SetObserver((Scene*)this);
	btnVsync->SetTexture(app->tex->Load("Assets/GUI/fullscreen.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"), app->tex->Load("Assets/GUI/fullscreen_on.png"));*/

	fxSliderBack = { 470,400,400,40 };

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool ScenePause::PreUpdate()
{
	return true;
}

bool ScenePause::Update(float dt)
{

	btnExit->Update(app->input, dt);
	/*fxSlider->Update(app->input, dt);
	btnConfig->Update(app->input, dt);
	btnVsync->Update(app->input, dt);*/
	return true;
}

// Update: draw background
bool ScenePause::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(configscreen, 0, 0, NULL);
	app->render->DrawRectangle(fxSliderBack, 154, 122, 130);
	btnExit->Draw(app->render);
	/*fxSlider->Draw(app->render);
	btnConfig->Draw(app->render);
	btnVsync->Draw(app->render);*/

	return true;
}

bool ScenePause::CleanUp()
{
	if (!active)return true;

	LOG("Freeing config");

	app->scenepause->active = false;

	app->tex->UnLoad(configscreen);

	return true;
}