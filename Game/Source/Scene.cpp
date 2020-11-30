#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Transition.h"
#include "Map.h"
#include "Collider.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module(), titleScene(nullptr),introScene(nullptr), deathScene(nullptr), currentScreen(Screens::TITLE_SCREEN)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{
}

// Called before render is available
bool Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");
	bool ret = true;
	sourceTitle = conf.child("title").attribute("name").as_string();
	sourceIntro = conf.child("intro").attribute("name").as_string();
	sourceDeath = conf.child("death").attribute("name").as_string();

	pugi::xml_node cp = conf.child("checkpoints");

	//Set Checkpoints

	checkpoint[0].rect.x = cp.child("cp1").attribute("x").as_int();
	checkpoint[0].rect.y = cp.child("cp1").attribute("y").as_int();

	checkpoint[1].rect.x = cp.child("cp2").attribute("x").as_int();
	checkpoint[1].rect.y = cp.child("cp2").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	titleScene = app->tex->Load(sourceTitle.GetString());
	introScene = app->tex->Load(sourceIntro.GetString());
	deathScene = app->tex->Load(sourceDeath.GetString());
	for (int i = 0; i < 2; i++)
	{
		checkpoint[i].Cp = (Cp)i;
		checkpoint[i].checked = false;
		checkpoint[i].rect.w = 10;
		checkpoint[i].rect.h = 20;

	}
	checkpoint[0].active = true;

	app->audio->PlayMusic("Assets/audio/music/Raxer_Sound_-_Pathfinder_Master.ogg");
	app->map->Load(app->map->GetLevelToLoad().GetString());
	
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
	//LOG("X: %d, Y: %d", app->player->playerInfo.position.x, app->player->playerInfo.position.y);
	/*
	if (checkpoint[0].checked) LOG("CP1 True");
	else
	{
		LOG("CP1 False");
	}

	if (checkpoint[1].checked) LOG("CP2 True");
	else
	{
		LOG("CP2 False");
	}
	*/

	switch (currentScreen)
	{
		case TITLE_SCREEN:
		{
			app->render->DrawTexture(titleScene, 0, 0);

			if (timer < 200)
			{
				timer++;
			}
			else
			{
				app->fade->FadeEffect(false, 30.0f);
				currentScreen = START_SCREEN;
			}

		}
		break;

		case START_SCREEN:
		{
		
			app->render->DrawTexture(introScene, 0, 0);

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				currentScreen = PLAYING;
			}
		}
		break;

		case DEAD_SCREEN:
		{
			if (checkpoint[0].checked) app->render->DrawTexture(deathScene, 788, 0);
			else if (checkpoint[1].checked) app->render->DrawTexture(deathScene, 255, 0);
			else { app->render->DrawTexture(deathScene, 0, 0); }

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				currentScreen = PLAYING;
				app->player->LoadCurrentLevel(app->player->playerInfo.currentLevel);
			}
		}
		break;

		case PLAYING:
		{
			if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				app->player->LoadCurrentLevel(LVL_1);
				app->scene->checkpoint[0].active = true;
				app->scene->checkpoint[1].active = false;
				
			}
			if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				app->player->LoadCurrentLevel(LVL_2);
				app->scene->checkpoint[0].active = false;
				app->scene->checkpoint[1].active = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				app->player->LoadCurrentLevel(app->player->playerInfo.currentLevel);
			}


			if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
				app->SaveRequest("savegame.xml");
			if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
				app->LoadRequest("savegame.xml");
			//L02: BONUS CODE
			if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
				app->audio->SetVolume(0);

			if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
				app->audio->SetVolume(1);
		}
		break;

		default:
			break;
	}
	if (app->player->godMode == false)
	{
		SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", app->map->mapInfo.width, app->map->mapInfo.height, app->map->mapInfo.tileWidth, app->map->mapInfo.tileHeight, app->map->mapInfo.tileSets.Count());

		app->win->SetTitle(title.GetString());
	}
	else
	{
		SString title("!!!!GOD MODE ACTIVATED!!!!");

		app->win->SetTitle(title.GetString());
	}


	if (checkpoint[0].active && app->collision->CheckCollision(app->player->playerColider, checkpoint[0].rect))
	{
		checkpoint[0].checked = true;
	}
	else if (checkpoint[1].active && app->collision->CheckCollision(app->player->playerColider, checkpoint[1].rect))
	{
		checkpoint[1].checked = true;
	}

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

