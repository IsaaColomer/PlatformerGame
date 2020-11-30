#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Animation.h"
#include "Intro.h"
#include "Scene.h"
#include "ModuleFadeToBlack.h"
#include "Scene2.h"
#include "Enemies.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->player->Init();
	app->player->Start();

	app->enemies->Init();
	app->enemies->Start();

	app->collisions->active = true;
	app->map->active = true;

	app->player->resetPlayer();

	foodRect = { 670,308,50,54 };
	foodCol = app->collisions->AddCollider(foodRect, Collider::Type::FOOD, this);
	foodAlive = true;

	flagRect = { 900,100,60,180 };
	flagCol = app->collisions->AddCollider(flagRect, Collider::Type::CHECKPOINT, this);
	flagAlive = true;

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	food = app->tex->Load("Assets/Screens/Gameplay/food.png");
	flag = app->tex->Load("Assets/Screens/Gameplay/flag.png");
	app->map->Load("map.tmx");
		
	//app->audio->PlayMusic("Assets/audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
		//camera movement
		//all draws
	app->render->DrawTexture(backg, 0, 0);
	app->map->Draw();
	app->render->DrawTexture(portal, 2325, 290);//2325
		// L03: DONE 7: Set the window title with map/tileset info
	app->map->LoadColliders();

	if (foodAlive)
	{
		app->render->DrawTexture(food, foodRect.x, foodRect.y, NULL);
	}

	if (flagAlive)
	{
		app->render->DrawTexture(flag, flagRect.x, flagRect.y, NULL);
	}

	return true;

}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	app->enemies->CleanUp();

	app->scene->active = false;

	LOG("Freeing scene");
	return true;
}