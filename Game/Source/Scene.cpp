#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Scene.h"
#include "Scene3.h"
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

	app->player->resetPlayer();

	app->collisions->active = true;
	app->map->active = true;


	//COINS LVL1
	app->entitymanager->AddEntity({ 419.0f,441.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 570.0f,308.0f }, Entity::Type::COIN);
	app->entitymanager->AddEntity({ 1154.0f,630.0f }, Entity::Type::COIN);

	app->entitymanager->AddEntity({ 1150.0f,400.0f }, Entity::Type::ENEMYG);
	app->entitymanager->AddEntity({ 1720.0f,607.0f }, Entity::Type::ENEMYG);

	app->entitymanager->AddEntity({ 2144.0f,600.0f }, Entity::Type::FOOD);

	flagRect = { 900,100,10,170 };
	flagCol = app->collisions->AddCollider(flagRect, Collider::Type::CHECKPOINT, this);
	flagAlive = true;

	backg = app->tex->Load("Assets/Screens/Gameplay/background.png");
	portal = app->tex->Load("Assets/Screens/Gameplay/portal.png");
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
	app->map->LoadColliders();

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
	app->entitymanager->DeleteEntity();
	app->entitymanager->CleanUp();

	app->tex->UnLoad(backg);
	app->tex->UnLoad(portal);
	app->tex->UnLoad(flag);
	app->tex->UnLoad(app->player->character);

	app->scene->active = false;
	app->scene2->active = false;
	app->scene3->active = false;

	LOG("Freeing scene");
	return true;
}