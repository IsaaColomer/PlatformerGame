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
#include "EnemyGround.h"
#include "Title.h"
#include "EntityManager.h"

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

	app->map->active = true;
	app->collisions->active = true;

	if (app->scene->doStart)
	{
		app->player->resetPlayer();
	}

	app->entitymanager->AddEntity({ 920.0F,272.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({1410.0f,551.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 2180.0f,338.0f }, Entity::Type::COIN);

	app->entitymanager->AddEntity({ 479.0f,471.0f }, Entity::Type::ENEMYG);

	app->entitymanager->AddEntity({ 1817.0f,481.0f }, Entity::Type::FOOD);

	flagRect = { 1150,100,10,180 };
	flagCol = app->collisions->AddCollider(flagRect, Collider::Type::CHECKPOINT, this);
	flagAlive = true;

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	flag = app->tex->Load("Assets/Screens/Gameplay/flag.png");
	app->map->Load("map2.tmx");

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
	app->render->DrawTexture(backg, 0, 0);
	app->map->Draw();
	app->render->DrawTexture(portal, 2370, 150);//2325

	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

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
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	app->entitymanager->DeleteEntity();
	app->entitymanager->CleanUp();

	app->tex->UnLoad(backg);
	app->tex->UnLoad(portal);
	app->tex->UnLoad(food);
	app->tex->UnLoad(flag);

	app->scene2->active = false;

	LOG("Freeing scene2");
	return true;
}