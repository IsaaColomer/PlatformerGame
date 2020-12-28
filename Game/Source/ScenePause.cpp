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

	LOG("Loading pause assets");

	scenepause = app->tex->Load("Assets/Screens/Title/config_screen.png");
	//app->audio->PlayMusic("Assets/Music/pornhubintro.mp3", 1.0f);

	btnExit = new GuiButton(10, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	btnResume = new GuiButton(11, { (1280 / 2 - 300 / 2), 180, 40, 40 }, "RESUME");
	btnResume->SetObserver((Scene*)this);
	btnResume->SetTexture(app->tex->Load("Assets/GUI/resume.png"), app->tex->Load("Assets/GUI/resume_selected.png"), app->tex->Load("Assets/GUI/resume_focused.png"));

	btnSettings = new GuiButton(12, { (1280 / 2 - 300 / 2), 240, 40, 40 }, "SETTINGS");
	btnSettings->SetObserver((Scene*)this);
	btnSettings->SetTexture(app->tex->Load("Assets/GUI/settings.png"), app->tex->Load("Assets/GUI/settings_selected.png"), app->tex->Load("Assets/GUI/settings_focused.png"));

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
	btnResume->Update(app->input, dt);
	btnSettings->Update(app->input, dt);
	return true;
}

// Update: draw background
bool ScenePause::PostUpdate()
{
	// Draw everything --------------------------------------
	btnExit->Draw(app->render);
	btnResume->Draw(app->render);
	btnSettings->Draw(app->render);

	return true;
}

bool ScenePause::CleanUp()
{
	if (!active)return true;

	LOG("Freeing config");

	app->scenepause->active = false;

	app->tex->UnLoad(scenepause);

	return true;
}