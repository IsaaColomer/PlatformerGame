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
#include "Scene2.h"
#include "Enemies.h"


#include "Defs.h"
#include "Log.h"

Scene2::Scene2() : Module()
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	app->player->Init();
	app->player->Start();

	app->enemies->Init();
	app->enemies->Start();

	app->enemies->ep.x = 80;
	app->enemies->ep.y = 500;


	app->map->active = true;
	app->collisions->active = true;

	app->player->resetPlayer();

	foodRect = { 300,100,50,54 };
	foodCol = app->collisions->AddCollider(foodRect, Collider::Type::FOOD, this);
	foodAlive = true;

	flagRect = { 1400,450,10,180 };
	flagCol = app->collisions->AddCollider(flagRect, Collider::Type::CHECKPOINT, this);
	flagAlive = true;

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	food = app->tex->Load("Assets/Screens/Gameplay/food.png");
	flag = app->tex->Load("Assets/Screens/Gameplay/flag.png");
	app->map->Load("map2.tmx");
	//app->audio->PlayMusic("Assets/audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{
	//camera movement
	//all draws
	app->render->DrawTexture(backg, 0, 0);
	app->map->Draw();
	app->render->DrawTexture(portal, 2370, 150);//2325
	// L03: DONE 7: Set the window title with map/tileset info
	app->map->LoadColliders();

	return true;

}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (foodAlive)
	{
		app->render->DrawTexture(food, foodRect.x, foodRect.y, NULL);
	}

	if (flagAlive)
	{
		app->render->DrawTexture(flag, flagRect.x, flagRect.y, NULL);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;	
	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	app->enemies->CleanUp();

	app->scene2->active = false;

	if (!active)
		return true;

	LOG("Freeing scene");
	return true;
}