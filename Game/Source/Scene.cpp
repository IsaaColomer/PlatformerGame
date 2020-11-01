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
	backg = app->tex->Load("Assets/map/background1.png");
	app->map->Load("mapend.tmx");
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	cpx = 70;
	cpy = 590;
	vcy = 0;
	ong = false;
	godMode = false;
	savedx = 70;
	savedy = 590;
	debugDraw = false;

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
	{
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			godMode = (godMode) ? false : true;
		}
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			cpx = 70;
			cpy = 590;
			vcy = 0;
			ong = false;
			app->render->camera.y = 0;
			app->render->camera.x = 0;
		}

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			app->LoadGameRequest();
			cpx = savedx;
			cpy = savedy;

		}
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveGameRequest();
			savedx = cpx;
			savedy = cpy;
		}

		//if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		//	app->render->camera.y += 1;

		//if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		//	app->render->camera.y -= 1;

		//if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		//	app->render->camera.x += 1;

		//if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		//	app->render->camera.x -= 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debugDraw = (debugDraw) ? false : true;
	}
	//player movement
	if (!godMode)
	{
		ong = false;
		for (int i = 0; i < 11 && !ong; i++)
		{
			if (((cpy + 110) > (coll[i][0] - 3)) && ((cpy + 110) < (coll[i][0] + 3)))
			{
				if (cpx<coll[i][1] && (cpx + 80)>coll[i][1])
					ong = true;
				else if (cpx<coll[i][2] && (cpx + 80)>coll[i][2])
					ong = true;
				else if (cpx > coll[i][1] && (cpx + 80) < coll[i][2])
					ong = true;
				else if (cpx<coll[i][1] && (cpx + 80)>coll[i][2])
					ong = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ong)
		{
			vcy = -6.0f;
			ong = false;
		}
		if (!ong)
		{
			if (vcy < 6.0f) vcy -= grav;
			cpy += vcy;
		}
		//if (ong)
		else vcy = 0;
	}
	
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			cpy -= 2.0f;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			cpy += 2.0f;
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cpx -= 2.0f;
		if (cpx > 640 && cpx < 1920)
		{
			app->render->camera.x += 2;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cpx += 2.0f;
		if (cpx > 640 && cpx <= 1920)
		{
		app->render->camera.x -= 2;
		}		
	}
	
	//all draws

	app->render->DrawTexture(backg, 0, 0);
	app->map->Draw();
	app->render->DrawTexture(character, cpx, cpy); // Placeholder not needed any more

	if (debugDraw)
	{
		SDL_Rect r;
		for (int i = 0; i < 11; i++)
		{
			r.x = coll[i][1];
			r.y = coll[i][0] - 2;
			r.w = coll[i][2] - coll[i][1];
			r.h = 4;
			
			app->render->DrawRectangle(r, 255, 0, 0, 128,true,true);
		}
		r.x = cpx;
		r.y = cpy;
		r.w = 80;
		r.h = 110;
		app->render->DrawRectangle(r, 255,0,0,128, true, true);
	}

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