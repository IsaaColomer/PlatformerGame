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
#include "CreditsScene.h"

#include "Defs.h"
#include "Log.h"

CreditsScene::CreditsScene() : Module()
{
	name.Create("credits");
}

CreditsScene::~CreditsScene()
{

}

bool CreditsScene::Awake()
{
	LOG("Loading Title");
	bool ret = true;

	return ret;
}

// Load assets
bool CreditsScene::Start()
{
	app->creditsscene->active = true;

	spaced = false;

	bool ret = true;

	LOG("Loading config assets");

	creditsScreen = app->tex->Load("Assets/Screens/Title/credits_screen.png");

	btnExit = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "EXIT");
	btnExit->SetObserver((Scene*)this);
	btnExit->SetTexture(app->tex->Load("Assets/GUI/exit.png"), app->tex->Load("Assets/GUI/exit_selected.png"), app->tex->Load("Assets/GUI/exit_focused.png"));

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool CreditsScene::PreUpdate()
{
	return true;
}

bool CreditsScene::Update(float dt)
{

	btnExit->Update(app->input, dt);
	return true;
}

// Update: draw background
bool CreditsScene::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(creditsScreen, 0, 0, NULL);
	btnExit->Draw(app->render);

	return true;
}

bool CreditsScene::CleanUp()
{
	if (!active)return true;

	LOG("Freeing credits");

	app->creditsscene->active = false;

	app->tex->UnLoad(creditsScreen);

	return true;
}