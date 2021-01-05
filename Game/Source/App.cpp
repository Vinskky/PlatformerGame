#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Pathfinding.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collider.h"
#include "Transition.h"
#include "GuiManager.h"
#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	pTimer.Start();

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	pathfinding = new PathFinding();
	scene = new Scene();
	map = new Map();
	enManager = new EntityManager();
	collision = new Collisions();
	fade = new Transition();
	guiManager = new GuiManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(pathfinding);
	AddModule(scene);
	AddModule(map);
	AddModule(enManager);
	AddModule(collision);
	AddModule(fade);
	AddModule(guiManager);


	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();

	configFile.reset();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	pTimer.Start();
	// L01: DONE 3: Load config from XML
	bool ret = LoadConfig();

	if(ret == true)
	{
		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	pTimer.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (changeFps) changeFps = false;
		else if (!changeFps) changeFps = true;
	}

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;
	dt = frameTime.ReadSec();
	frameTime.Start();
}

void App::FinishUpdate()
{
	// L02: TODO 1: This is a good place to call Load / Save methods
	if (loadRequest == true)
		LoadGame();
	if (saveRequest == true)
		SaveGame();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;
	uint32 lastFrameMs = 0;
	uint32 framesOnLastUpdate = 0;

	averageFps = float(frameCount) / startupTime.ReadSec();
	secondsSinceStartup = startupTime.ReadSec();
	lastFrameMs = frameTime.Read();
	framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f | Last Frame Ms: %02u | Last Sec Frames: %i | Last dt: %.3f | Time Since Startup: %.3f | Frame Count: %I64u",
		averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);
	
	if(!app->enManager->player->godMode) app->win->SetTitle(title);

	if (frameDelay > lastFrameMs && !changeFps)
	{
		SDL_Delay(frameDelay - lastFrameMs);
	}
	if ((1000.0f/30.0f) > lastFrameMs && changeFps)
	{
		SDL_Delay(1000.0f / 30.0f - lastFrameMs);
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{
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

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
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

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	pTimer.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.GetString();
}

const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadRequest(const char* filename)
{
	loadRequest = true;
	loadFileName = filename;
}

void App::SaveRequest(const char* filename)
{
	saveRequest = true;
	saveFileName = filename;
}
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;
	pugi::xml_parse_result doc = data.load_file(loadFileName.GetString());

	if (doc == NULL)
	{
		LOG("There was an arror trying to load the game, take care of save_game.xml file. %s", doc.description());
	}
	else
	{
		root = data.child("saveState");
		ret = true;

		ListItem<Module*>* item;
		item = modules.end;

		while (item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			if (ret == false)
			{
				LOG("Error loading module: %s", item->data->name.GetString());
			}
				
			item = item->prev;
			
		}

		//Renderer loaded apart
		ret = app->render->Load(root.child("renderer"));
		if (ret == false)
		{
			LOG("Error loading module: %s", app->render->name.GetString());
		}

		LOG("Loading finished...");
	}

	loadRequest = false;
	return ret;
}

bool App::IsLoading() const
{
	return loadRequest;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{

	pugi::xml_document saveDoc;
	pugi::xml_node root = saveDoc.append_child("saveState");
	pugi::xml_node rootChild;

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	if (root != NULL)
	{
		while (item != NULL && ret == true)
		{
			rootChild = root.append_child(item->data->name.GetString());
			ret = item->data->Save(rootChild);
			if (ret == false)
			{
				LOG("Error on save method from module: %s", item->data->name.GetString());
			}
			item = item->prev;
		}

		saveDoc.save_file(saveFileName.GetString());
		LOG("Game saved correctly");
	}
	else
	{
		LOG("Error on append child of the savegame.xml file");
		ret = false;
	}
	
	saveRequest = false;
	return ret;
}

int App::GetFrameCount()
{
	return (int)frameCount;
}




