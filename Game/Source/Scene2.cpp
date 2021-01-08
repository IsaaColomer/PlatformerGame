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
#include "ScenePause.h"
#include "FadeToBlack.h"
#include "Scene3.h"

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
	app->player->sceneTwoA = true;
	app->player->sceneOneA = false;
	app->player->sceneThreeA = false;

	app->player->Init();
	app->player->Start();

	app->scene->sceneOnScreen = false;
	app->scene2->scene2OnScreen = true;
	app->scene3->scene3OnScreen = false;

	if (tpToScene2)
	{
		app->player->cp.x = 1150;
		app->player->cp.y = 170;
	}

	app->map->active = true;
	app->collisions->active = true;


	app->entitymanager->AddEntity({ 920.0F,272.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({1410.0f,551.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 2180.0f,338.0f }, Entity::Type::COIN);

	app->entitymanager->AddEntity({ 1817.0f,481.0f }, Entity::Type::FOOD);

	app->entitymanager->AddEntity({ 1150,110 }, Entity::Type::FLAG);

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	//flag = app->tex->Load("Assets/Screens/Gameplay/flag.png");
	checked = app->tex->Load("Assets/Screens/Gameplay/checked.png");
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
	//all draws

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
	char score[64] = { 0 };
	sprintf_s(score, 64, "Timer: %d", 56);

	app->render->DrawText(app->render->font, score, 1025, 0, 50, 5, { 255, 255, 43, 255 });
	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	app->player->sceneTwoA = false;
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