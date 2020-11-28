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

		app->player->cp.x = 70;//70
		app->player->cp.y = 0;//500
		app->player->cp.w = 66;
		app->player->cp.h = 110;
		app->player->vcy = 0;
		app->player->vcx = 2.0f;

		app->player->xMove = false;
		app->player->ong = false;

		app->render->camera.y = 0;
		app->render->camera.x = 0;

		backg = app->tex->Load("Assets/Gameplay/background.png");
		portal = app->tex->Load("Assets/Map/portal.png");
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
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->data.width, app->map->data.height,
		app->map->data.tileWidth, app->map->data.tileHeight,
		app->map->data.tilesets.count());

	app->win->SetTitle(title.GetString());

	app->map->LoadColliders();

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