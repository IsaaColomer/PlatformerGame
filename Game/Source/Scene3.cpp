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
#include "Scene3.h"
#include "EnemyGround.h"
#include "Title.h"
#include "EntityManager.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene3::Scene3() : Module()
{
	name.Create("scene3");
}

// Destructor
Scene3::~Scene3()
{}

// Called before render is available
bool Scene3::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene3::Start()
{
	app->player->sceneThreeA = true;
	app->player->sceneTwoA = false;
	app->player->sceneOneA = false;

	app->player->Init();
	app->player->Start();

	app->scene->sceneOnScreen = false;
	app->scene2->scene2OnScreen = false;
	app->scene3->scene3OnScreen = true;

	app->map->active = true;
	app->collisions->active = true;

	app->entitymanager->AddEntity({ 1000.0f,551.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 1764.0f,227.0f }, Entity::Type::COIN);

	app->entitymanager->AddEntity({ 1415.0f,301.0f }, Entity::Type::FOOD );

	app->entitymanager->AddEntity({ 1000.0f,250.0f }, Entity::Type::ENEMYA);

	app->entitymanager->AddEntity({ 2000.0f,200.0f }, Entity::Type::ENEMYA);

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
	app->map->Load("map3.tmx");
	//app->audio->PlayMusic("Assets/audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene3::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene3::Update(float dt)
{
	//all draws

	app->render->DrawTexture(backg, 0, 0);
	app->map->Draw();
	app->render->DrawTexture(portal, 2340, 220);//2325

	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene3::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene3::CleanUp()
{
	app->player->sceneThreeA = false;
	if (!active)return true;

	app->map->CleanUp();
	app->player->CleanUp();
	app->collisions->CleanUp();

	app->entitymanager->DeleteEntity();
	app->entitymanager->CleanUp();

	app->tex->UnLoad(backg);
	app->tex->UnLoad(portal);
	app->tex->UnLoad(food);

	app->scene3->active = false;

	LOG("Freeing scene3");
	return true;
}