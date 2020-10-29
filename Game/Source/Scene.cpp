#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

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
	character = app->tex->Load("Assets/player/adventurer_stand.png");
	backg = app->tex->Load("Assets/map/background.png");
	app->map->Load("map.tmx");
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	cpx = 70;
	cpy = 590;
	vcy = 0;

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
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	//player movement
	vcy -= grav;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		vcy = -0.7f;

	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
	{
		cpx -= 0.1f;
		app->render->camera.x += 1;
		if (cpx <= 640.0f)
		{
			app->render->camera.x -= 1;
		}
	}
		

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		cpx += 0.1f;
		if (cpx >= 640.0f)
		{
			app->render->camera.x -= 1;
		}
	}
	if (cpx <= 640.0f)
	{
		app->render->camera.x = 0;
	}
	if (cpx >= 1850.0f)
	{
		app->render->camera.x = -12300.0f;
	}

	
	cpy += vcy;
	if (cpy > 590) cpy = 590;


	for (int i = 0; i < 10; i++)
	{
		if (cpy<coll[i][0] && cpx>coll[i][1] && cpx < coll[i][2])
		{
			cpy = coll[i][0] - 110;
			i = 10;
		}
	}

	//all draws

	app->render->DrawTexture(backg, 0, 0);
	//SDL_SetRenderDrawColor(app->render->renderer, 65, 205, 186, 255);
	//SDL_RenderClear(app->render->renderer);

	app->map->Draw();
	app->render->DrawTexture(character, cpx, cpy); // Placeholder not needed any more

	// Draw map

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->data.width, app->map->data.height,
		app->map->data.tileWidth, app->map->data.tileHeight,
		app->map->data.tilesets.count());

	app->win->SetTitle(title.GetString());

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
	LOG("Freeing scene");

	return true;
}
