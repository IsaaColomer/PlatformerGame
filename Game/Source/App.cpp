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
#include "Enemies.h"
#include "Collectible.h"

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
	PERF_START(perfTimer);

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
	enemies = new Enemies();
	collectible = new Collectible();

	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(collisions);
	AddModule(map);
	AddModule(player);
	AddModule(intro);
	AddModule(scene);
	AddModule(scene2);
	AddModule(fade);
	AddModule(titleScreen);
	AddModule(collectible);
	//AddModule(enemies);

	scene->active = false;
	player->active = false;
	collisions->active = false;
	scene2->active = false;
	intro->active = false;
	enemies->active = false;

	// Render last to swap buffer
	AddModule(render);
	//LOG("It took %f ms to execute", startupTime.Read());
	PERF_PEEK(perfTimer);
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
	PERF_START(perfTimer);
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
		//READ CONFIG FILE OUR FRAMERATE
		frameRate = configApp.attribute("framerate").as_int(-1);
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	pugi::xml_parse_result result = saveLoadFile.load_file("savegame.xml");
	if (result != NULL)
	{
		saveLoadNode = saveLoadFile.child("save");
	}

	PERF_PEEK(perfTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(perfTimer);

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
	fpsCount++;
	lastSecFrameCount++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	uint32 lastFrameInMs = 0;
	uint32 framesOnLastUpdate = 0;

	float secondsStart = startTime.ReadSec();
	float average = fpsCount / secondsStart;

	if (frameTime.ReadSec() > 1.0f)
	{
		framesSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		frameTime.Start();
	}

	oldLastFrame = lastFrameInMs;

	lastFrameInMs = lastSec.Read();

	lastSec.Start();

	int delay = (1000 * (1.0f / frameRate));

	if (lastFrameInMs < 1000 * (1.0f / frameRate))
	{
		perfTimer.Start();
		SDL_Delay(delay);
		timePerfect = perfTimer.ReadMs();
	}
}

// Call modules before each loop iteration
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

// Call modules on each loop iteration
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

// Call modules after each loop iteration
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
// Called before quitting
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
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = true;

	ListItem<Module*>* item;
	item = modules.start;
	
	while (item != NULL && ret == true)
	{
		ret = item->data->LoadState(saveLoadNode.child(item->data->name.GetString()));
		item = item->next;
	}
//	saveLoadFile.reset();

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->SaveState(saveLoadNode.child(item->data->name.GetString()));
		item = item->next;
	}
	saveLoadFile.save_file("savegame.xml");

	saveGameRequested = false;

	return ret;
}