#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Collisions.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "Intro.h"
#include "Scene2.h"
#include "Title.h"
#include "EnemyGround.h"
#include "EntityManager.h"
#include "Scene3.h"
#include "Pathfinding.h"
#include "ConfigScene.h"
#include "CreditsScene.h"
#include "ScenePause.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// L07: TODO 3: Measure the amount of ms that takes to execute:
// App constructor, Awake, Start and CleanUp
// LOG the result

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	//PERF_START(perfTimer);

	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	collisions = new Collisions(true);
	scene = new Scene();
	map = new Map();
	player = new Player();
	fade = new FadeToBlack();
	intro = new Intro();
	scene2 = new Scene2();
	titleScreen = new Title();
	entitymanager = new EntityManager();
	scene3 = new Scene3();
	pathfinding = new PathFinding();
	configscene = new ConfigScene();
	creditsscene = new CreditsScene();
	scenepause = new ScenePause();

	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(collisions);
	AddModule(map);
	AddModule(fade);
	AddModule(player);
	AddModule(intro);
	AddModule(scene);
	AddModule(scene2);
	AddModule(titleScreen);
	AddModule(entitymanager);
	AddModule(scene3);
	AddModule(pathfinding);
	AddModule(configscene);
	AddModule(creditsscene);
	AddModule(scenepause);
	//AddModule(enemies);


	scene->active = false;
	scene2->active = false;
	scene3->active = false;
	scenepause->active = false;
	creditsscene->active = false;
	configscene->active = false;
	player->active = false;
	collisions->active = false;
	intro->active = false;

	// Render last to swap buffer
	AddModule(render);
	//LOG("It took %f ms to execute", startupTime.Read());
	//PERF_PEEK(perfTimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	//PERF_START(perfTimer);
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		//READ CONFIG.XML TO LET THE CODE KNOW THE FRAMERATE
		//frameRate60 = configApp.attribute("framerate").as_int(-1);
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	pugi::xml_parse_result result = saveLoadFile.load_file("save_game.xml");
	if (result != NULL)
	{
		saveLoadNode = saveLoadFile.child("save");
		savedFile = true;
	}
	if (result == NULL)
	{
		savedFile = false;
	}

	//PERF_PEEK(perfTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(perfTimer);
	lastSec.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
		{
			ret = item->data->Start();
		}
		item = item->next;
	}
	PERF_PEEK(perfTimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		cappedFrameRate = !cappedFrameRate;
	}

	dt = frameTime.ReadSec();
	if (dt > 0.05)
		dt = 0.033;
	frameTime.Start();

	fpsCount++;
	lastSecFrameCount++;

	startFrameTimeMs = SDL_GetTicks();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	if (lastSec.Read() > 1000)
	{
		previousLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
		lastSec.Start();
	}

	fpsAverageSinceStart = fpsCount / startTime.ReadSec();

	if (dt < 1000/30 && cappedFrameRate)
	{
		float delay = 1000/30 - dt;
		SDL_Delay(delay);
	}

	static char title[256];
	sprintf_s(title, 256, "FPS: %d | AVG FPS %.2f | Last Frame in ms: %.1f(dt) | VSync = On ",
		previousLastSecFrameCount, fpsAverageSinceStart, dt*1000.0);
	app->win->SetTitle(title);
}

bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}
	

	return ret;
}

bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false)
		{
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

bool App::CleanUp()
{
	startTime.Start();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	lastSecFrameCount += startTime.Read();

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	app->savedFile = true;
	saveGameRequested = true;
}

// ---------------------------------------
bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_document savedGame;

	pugi::xml_parse_result result = savedGame.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load saved game xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		//General Node
		pugi::xml_node generalNode = savedGame.child("save");

		//Player
		pugi::xml_node player = generalNode.child("player");

		//Entities
		pugi::xml_node entities = generalNode.child("entities");


		//LoadRequests
		app->player->LoadState(player);
	}

	loadGameRequested = false;

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	saveGameRequested = false;

	ListItem<Module*>* item;
	item = modules.start;

	pugi::xml_document newSave;
	pugi::xml_node save;

	save = newSave.append_child("save");

	pugi::xml_node player = save.append_child("player");
	app->player->SaveState(player);

	pugi::xml_node entities = save.append_child("entities");

	newSave.save_file("save_game.xml");


	return ret;

}