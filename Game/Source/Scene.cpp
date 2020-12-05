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
#include "Pathfinding.h"
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
	checkpoint[0].source.Create(cp.attribute("name").as_string());
	checkpoint[1].source.Create(cp.attribute("name").as_string());

	checkpoint[0].rect.x = cp.child("cp1").attribute("x").as_int();
	checkpoint[0].rect.y = cp.child("cp1").attribute("y").as_int();

	checkpoint[1].rect.x = cp.child("cp2").attribute("x").as_int();
	checkpoint[1].rect.y = cp.child("cp2").attribute("y").as_int();

	cp = conf.child("collectible");

	for (int i = 0; i < 4; i++)
	{
		collectible[i].source.Create(cp.attribute("name").as_string());
		collectible[i].itemRect.w = cp.attribute("w").as_int();
		collectible[i].itemRect.h = cp.attribute("h").as_int();
	}
	int i = 0;
	for (pugi::xml_node collect = cp.child("collect"); collect; collect = collect.next_sibling("collect"))
	{
		collectible[i].itemRect.x = collect.attribute("x").as_int();
		collectible[i].itemRect.y = collect.attribute("y").as_int();
		collectible[i].active = collect.attribute("active").as_bool();
		i++;
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//CP
	checkpoint[0].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());
	checkpoint[1].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());

	for (int i = 0; i < 2; i++)
	{
		checkpoint[i].cp = (Cp)i;
		checkpoint[i].checked = false;
		checkpoint[i].rect.w = 22;
		checkpoint[i].rect.h = 22;
	}
	checkpoint[0].active = true;

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		collectible[i].itemTex = app->tex->Load(collectible[i].source.GetString());
	}

	//SCREENS
	titleScene = app->tex->Load(sourceTitle.GetString());
	introScene = app->tex->Load(sourceIntro.GetString());
	deathScene = app->tex->Load(sourceDeath.GetString());

	app->audio->PlayMusic("Assets/audio/music/Raxer_Sound_-_Pathfinder_Master.ogg");
	if (app->map->Load(app->map->GetLevelToLoad().GetString()) == true)
	{
		int w, h;
		uchar* data = NULL;
		
		if(app->map->CreateWalkabilityMap(&w, &h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	
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
			//DIESCREEN
			if (checkpoint[0].checked) app->render->DrawTexture(deathScene, 788, 0);
			else if (checkpoint[1].checked) app->render->DrawTexture(deathScene, 255, 0);
			else { app->render->DrawTexture(deathScene, 0, 0); }
			
			//LIVES
			app->player->playerLife.lifes = 3;

			//COLLECIBLES
			if (!checkpoint[0].checked && app->player->playerInfo.currentLevel == LVL_1)
			{
				collectible[0].collected = false;
			}
			else if (checkpoint[0].checked && app->player->playerInfo.currentLevel == LVL_1)
			{
				collectible[1].collected = false;
			}
			if (!checkpoint[1].checked && app->player->playerInfo.currentLevel == LVL_2)
			{
				collectible[2].collected = false;
			}
			else if (checkpoint[1].checked && app->player->playerInfo.currentLevel == LVL_2)
			{
				collectible[3].collected = false;
			}

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

				//RESTART COLLECTIBLES
				for (int i = 0; i < 4; i++) collectible[i].collected = false;

				app->player->LoadCurrentLevel(LVL_1);

				//ACTIVATE CHECKPOINTS
				app->scene->checkpoint[0].active = true;
				app->scene->checkpoint[1].active = false;

				//ACTIVATE COLLECTIBLES
				collectible[0].active = true;
				collectible[1].active = true;
				collectible[2].active = false;
				collectible[3].active = false;
			}
			if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				//RESTART COLLECTIBLES
				for (int i = 0; i < 4; i++) collectible[i].collected = false;

				app->player->LoadCurrentLevel(LVL_2);

				//ACTIVATE CHECKPOINTS
				app->scene->checkpoint[0].active = false;
				app->scene->checkpoint[1].active = true;

				//ACTIVATE COLLECTIBLES
				collectible[0].active = false;
				collectible[1].active = false;
				collectible[2].active = true;
				collectible[3].active = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				//RESTART COLLECTIBLES
				for (int i = 0; i < 4; i++) collectible[i].collected = false;

				app->player->LoadCurrentLevel(app->player->playerInfo.currentLevel);
			}
			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				app->player->TP(CP1);
			}
			if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				//RESTART CHECKPOINTS
				app->scene->checkpoint[0].checked = false;
				app->scene->checkpoint[1].checked = false;

				app->player->TP(CP2);
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
		//SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d", app->map->mapInfo.width, app->map->mapInfo.height, app->map->mapInfo.tileWidth, app->map->mapInfo.tileHeight, app->map->mapInfo.tileSets.Count());

		//app->win->SetTitle(title.GetString());
	}
	else
	{
		SString title("!!!!GOD MODE ACTIVATED!!!!");

		app->win->SetTitle(title.GetString());
	}

	if (checkpoint[0].active && app->collision->CheckCollision(app->player->playerColider, checkpoint[0].rect))
	{
		if (checkpoint[0].checked == false)
		{
			app->audio->PlayFx(2);
		}
		checkpoint[0].checked = true;
	}
	else if (checkpoint[1].active && app->collision->CheckCollision(app->player->playerColider, checkpoint[1].rect))
	{
		if (checkpoint[1].checked == false)
		{
			app->audio->PlayFx(2);
		}
		checkpoint[1].checked = true;
	}

	for (int i = 0; i < 4; i++)
	{
		if (collectible[i].active && app->collision->CheckCollision(app->player->playerColider, collectible[i].itemRect))
		{
			if (collectible[i].collected == false)
			{
				app->audio->PlayFx(3);
			}
			collectible[i].collected = true;
		}
	}

	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	const SDL_Rect sectionCPAnim1 = { 0, 0, 22, 22 };
	const SDL_Rect sectionCPAnim2 = { 22, 0, 22, 22 };

	if (checkpoint[0].active && !checkpoint[0].checked && currentScreen != DEAD_SCREEN)
	{
		app->render->DrawTexture(checkpoint[0].checkpointTex, checkpoint[0].rect.x, checkpoint[0].rect.y, &sectionCPAnim1);
	}
	else if (checkpoint[0].active && checkpoint[0].checked && currentScreen != DEAD_SCREEN)
	{
		app->render->DrawTexture(checkpoint[0].checkpointTex, checkpoint[0].rect.x, checkpoint[0].rect.y, &sectionCPAnim2);
	}

	if (checkpoint[1].active && !checkpoint[1].checked && currentScreen != DEAD_SCREEN)
	{
		app->render->DrawTexture(checkpoint[1].checkpointTex, checkpoint[1].rect.x, checkpoint[1].rect.y, &sectionCPAnim1);
	}
	else if (checkpoint[1].active && checkpoint[1].checked && currentScreen != DEAD_SCREEN)
	{
		app->render->DrawTexture(checkpoint[1].checkpointTex, checkpoint[1].rect.x, checkpoint[1].rect.y, &sectionCPAnim2);
	}

	for (int i = 0; i < 4; i++)
	{
		if (collectible[i].active && !collectible[i].collected && currentScreen != DEAD_SCREEN)
		{
			app->render->DrawTexture(collectible[i].itemTex, collectible[i].itemRect.x, collectible[i].itemRect.y);
		}
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
