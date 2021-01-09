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
#include "FadeToBlack.h"
#include "Scene2.h"
#include "EnemyGround.h"
#include "Title.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include "Scene3.h"
#include "Font.h"
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
	app->player->sceneOneA = true;
	app->player->sceneTwoA = false;
	app->player->sceneThreeA = false;

	sceneOnScreen = true;
	app->scene2->scene2OnScreen = false;
	app->scene3->scene3OnScreen = false;

	app->player->Init();
	app->player->Start();

	timer.Start();

	if (app->scene->tpToScene)
	{
		app->player->cp.x = 900;
		app->player->cp.y = 170;
	}
	app->collisions->active = true;
	app->map->active = true;

	if (app->player->sceneOneA)
	{
		app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg", 1.0f);
	}
	
	int w, h;
	uchar* data = NULL;

	if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	//COINS LVL1
	app->entitymanager->AddEntity({ 419.0f,441.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 570.0f,308.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 1154.0f,630.0f }, Entity::Type::COIN);

	app->entitymanager->AddEntity({ 1150.0f,607.0f }, Entity::Type::ENEMYG);
	//app->entitymanager->AddEntity({ 900.0f,100.0f }, Entity::Type::ENEMYG);
	app->entitymanager->AddEntity({ 1720.0f,607.0f }, Entity::Type::ENEMYG);

	app->entitymanager->AddEntity({ 1720.0f,100.0f }, Entity::Type::ENEMYG);

	app->entitymanager->AddEntity({ 2144.0f,600.0f }, Entity::Type::FOOD);

	app->entitymanager->AddEntity({ 2144.0f,600.0f }, Entity::Type::FOOD);

	app->entitymanager->AddEntity({ 900,110 }, Entity::Type::FLAG);

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	checked = app->tex->Load("Assets/Screens/Gameplay/checked.png");

	app->map->Load("map.tmx");
	
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
	app->render->DrawTexture(backg, 0, 0,NULL);
	app->map->Draw();
	app->render->DrawTexture(portal, 2325, 290);//2325
	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	char score[64] = { 0 };
	int time = timer.ReadSec();

	sprintf_s(score, 64, "Time: %d", time);
	app->render->DrawText(app->render->font, score, 1000, 0, 50, 5, { 255, 255, 43, 255 });

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	app->player->sceneOneA = false;
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();
	//app->audio->CleanUp();
	app->entitymanager->DeleteEntity();

	app->tex->UnLoad(backg);
	app->tex->UnLoad(portal);
	app->tex->UnLoad(flag);

	app->scene->active = false;

	LOG("Freeing scene");
	return true;
}