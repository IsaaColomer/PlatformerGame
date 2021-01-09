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

	btnBack = new GuiButton(10, { 60, 29, 78, 23 }, "BACK");
	btnBack->SetObserver((Scene*)this);
	btnBack->SetTexture(app->tex->Load("Assets/GUI/back.png"), app->tex->Load("Assets/GUI/back_selected.png"), app->tex->Load("Assets/GUI/back_focused.png"));

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
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->intro->debug = !app->intro->debug;
	}
	btnBack->Update(app->input, dt);
	return true;
}

// Update: draw background
bool CreditsScene::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(creditsScreen, 0, 0, NULL);
	btnBack->Draw(app->render);

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