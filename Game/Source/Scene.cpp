#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
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
	//img = app->tex->Load("Assets/textures/test.png");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	app->map->Load(app->map->GetLevel2Load().GetString());
	
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
    // L02: TODO 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadRequest("savegame.xml");
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveRequest("savegame.xml");
	//L02: BONUS CODE
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		app->audio->SetVolume(0);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->SetVolume(1);

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		app->player->playerInfo.position.x -= 1;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		app->player->playerInfo.position.x += 1;
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->player->Jump();

	app->map->Draw();
	app->player->Draw();
	//app->render->DrawTexture(img, 380, 100);
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", app->map->mapInfo.width, app->map->mapInfo.height, app->map->mapInfo.tileWidth, app->map->mapInfo.tileHeight, app->map->mapInfo.tileSets.count());

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
