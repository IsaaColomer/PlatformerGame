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

#include "Defs.h"
#include "Log.h"

ConfigScene::ConfigScene() : Module()
{
	name.Create("title");
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

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);

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
	return true;
}

// Update: draw background
bool ConfigScene::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(configscreen, 0, 0, NULL);
	btnExit->Draw(app->render);
	return true;
}

bool ConfigScene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 4) app->fade->Fade((Module*)app->configscene, (Module*)app->intro, 1);
	}
	default: break;
	}

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