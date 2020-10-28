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

#define MAX_RIGHT_DISTANCE 320
#define MAX_LEFT_DISTANCE 50

Scene::Scene() : Module(), introKey(true),introScene(nullptr), deathScene(nullptr)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");
	bool ret = true;
	sourceIntro = conf.child("intro").attribute("name").as_string();
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	introScene = app->tex->Load(sourceIntro.GetString());
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
	if (introKey == false)
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

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			app->player->playerInfo.position.x -= 1;
			if (app->render->camera.x < 0 /*&& app->player->playerInfo.position.x <= MAX_LEFT_DISTANCE*/) {
				app->render->camera.x += 3;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			app->player->playerInfo.position.x += 1;
			if (app->render->camera.x > -2559 /*&& app->player->playerInfo.position.x >= MAX_RIGHT_DISTANCE*/) {
				app->render->camera.x -= 3;
			}

		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			ListItem<MapLayer*>* item = app->map->mapInfo.layers.start;
			while (item != NULL)
			{
				if (item->data->name == "Colision Layer")
				{
					ListItem<Properties::Property*>* item2 = item->data->properties.list.start;
					while (item2 != NULL)
					{
						if (item2->data->name == "Drawable")
						{
							if (item2->data->value == "true")
							{
								item2->data->value = "false";
							}
							else
							{
								item2->data->value = "true";
							}
						}
						item2 = item2->next;
					}
				}
				item = item->next;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			app->player->Jump();

		app->map->Draw();
		app->player->Draw();

		
	}
	else
	{
		app->render->DrawTexture(introScene, 0, 0);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			introKey = false;
	}
    
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
