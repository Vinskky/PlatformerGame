#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"
#include "Timer.h"
#include "PerfTimer.h"


// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class PathFinding;
class EntityManager;
class Collisions;
class Transition;
class GuiManager;
class Fonts;

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

    // L02: TODO 1: Create methods to request Load / Save
	void LoadRequest(const char* filename);
	void SaveRequest(const char* filename);
	bool IsLoading() const;

	int GetFrameCount();

private:

	// Load config file
	bool LoadConfig();

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

	// L02: TODO 5: Declare methods to load/save game
	bool LoadGame();
	
	bool SaveGame() const;
public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Map* map;
	PathFinding* pathfinding;
	Collisions* collision;
	Transition* fade;
	EntityManager* enManager;
	GuiManager* guiManager;
	Fonts* font;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;

	// L02: TODO 1: Create required variables to request load / save and 
	// the filename for save / load
	bool loadRequest = false;
	mutable bool saveRequest = false;
	SString loadFileName;
	mutable SString saveFileName;

	// FRAME CAP SHENANIGANS
	PerfTimer pTimer;
	uint64 frameCount = 0;
	const uint32 fps = 60;
	const uint32 frameDelay = 1000 / fps;
	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;
	bool changeFps = false;
};

extern App* app;

#endif	// __APP_H__