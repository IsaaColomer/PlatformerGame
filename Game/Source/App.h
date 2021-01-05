#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Collisions;
class Scene;
class Map;
class Player;
class FadeToBlack;
class Intro;
class Scene2;
class Title;
class EnemyGround;
class EntityManager;
class Scene3;
class PathFinding;
class ConfigScene;
class CreditsScene;
class ScenePause;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGameRequest();
	void SaveGameRequest() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Collisions* collisions;
	Scene* scene;
	Map* map;
	Player* player;
	FadeToBlack* fade;
	Intro* intro;
	Scene2* scene2;
	Scene3* scene3;
	Title* titleScreen;
	EnemyGround* enemies;
	EntityManager* entitymanager;
	PathFinding* pathfinding;
	ConfigScene* configscene;
	CreditsScene* creditsscene;
	ScenePause* scenepause;

	bool escaped;
	bool cappedFrameRate = false;
private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	Timer startTime;
	Timer frameTime;
	Timer lastSec;
	float dt = 0.0f;
	float startFrameTimeMs = 0.0f;
	uint32 lastSecFrameCount = 0;
	uint32 previousLastSecFrameCount = 0;

	uint frames;
	PerfTimer perfTimer;
	uint64 fpsCount = 0;

	uint32 fpsEverySec = 0;
	float fpsAverageSinceStart = 0.0f;


	mutable bool saveGameRequested;
	bool loadGameRequested;

	pugi::xml_document saveLoadFile;
	pugi::xml_node saveLoadNode;
};

extern App* app;

#endif	// __APP_H__