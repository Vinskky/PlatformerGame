#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Transition.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Collider.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module(), titleScene(nullptr),menuScene(nullptr), deathScene(nullptr), currentScreen(Screens::TITLE_SCREEN)
{
	name.Create("scene");
}

Scene::~Scene()
{
}

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

	cp = conf.child("collectiblemark");

	sourceMarker.Create(cp.attribute("name").as_string());

	return ret;
}

bool Scene::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/raxer_sound_pathfinder_master.ogg");
	if (app->map->Load(app->map->GetLevelToLoad().GetString()) == true)
	{
		int w, h;
		uchar* data = NULL;
		
		if(app->map->CreateWalkabilityMap(&w, &h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	for (int i = 0; i < 2; i++)
	{
		checkpoint[i].cp = (Cp)i;
		checkpoint[i].checked = false;
		checkpoint[i].rect.w = 22;
		checkpoint[i].rect.h = 22;
	}
	checkpoint[0].active = true;

	SetScene(TITLE_SCREEN);
	
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	if (currentScreen == TITLE_SCREEN)
		UpdateTitle();

	else if (currentScreen == MAIN_MENU)
		UpdateMainMenu();

	else if (currentScreen == DEAD_SCREEN)
		UpdateDeadScreen();

	else if (currentScreen == LVL1 || currentScreen == LVL2)
		UpdateLevels();

	else if (currentScreen == CONFIG_MENU)
		UpdateConfigMenu();

	else if (currentScreen == PAUSE_MENU)
		UpdatePauseMenu();

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::CollectibleMarkerLogic()
{
		for (int i = 0; i < 4; i++)
		{
			if (collectible[i].collected)
			{
				collectibleCount++;
			}
		}

		if (collectibleCount == 1)
		{
			app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
		}
		else if (collectibleCount == 2)
		{
			app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
		}
		else if (collectibleCount == 3)
		{
			app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 70 - app->render->camera.x / 3, 3);
		}
		else if (collectibleCount == 4)
		{
			app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 70 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(markerTex, 426 - 90 - app->render->camera.x / 3, 3);
		}

		collectibleCount = 0;
}


//------------------------------------------------------------------------------------------------------------
//-----------------------------------------------SCREEN MANAGER-----------------------------------------------
//------------------------------------------------------------------------------------------------------------

void Scene::SceneCleanUp()
{
	//SCREENS UNLOAD
	if (titleScene != nullptr && titleScene != NULL)
		app->tex->UnLoad(titleScene);

	if (deathScene != nullptr && deathScene != NULL)
		app->tex->UnLoad(deathScene);

	if (menuScene != nullptr && menuScene != NULL)
		app->tex->UnLoad(menuScene);

	if (configMenu != nullptr && configMenu != NULL)
		app->tex->UnLoad(configMenu);

	if (pauseMenu != nullptr && pauseMenu != NULL)
		app->tex->UnLoad(pauseMenu);

	//GUI UNLOAD
	if (app->enManager->player->playerLife.lifeTex != nullptr && app->enManager->player->playerLife.lifeTex != NULL)
		app->tex->UnLoad(app->enManager->player->playerLife.lifeTex);

	if (markerTex != nullptr && markerTex != NULL)
		app->tex->UnLoad(markerTex);

	//ENTITIES UNLOAD
	if (app->enManager->player->lifeGetter[0].getterTex != nullptr && app->enManager->player->lifeGetter[0].getterTex != NULL)
		app->tex->UnLoad(app->enManager->player->lifeGetter[0].getterTex);

	if (app->enManager->player->lifeGetter[1].getterTex != nullptr && app->enManager->player->lifeGetter[1].getterTex != NULL)
		app->tex->UnLoad(app->enManager->player->lifeGetter[1].getterTex);

	if (app->enManager->player->texture != nullptr && app->enManager->player->texture != NULL)
		app->tex->UnLoad(app->enManager->player->texture);

	if (checkpoint[0].checkpointTex != nullptr && checkpoint[0].checkpointTex != NULL)
		app->tex->UnLoad(checkpoint[0].checkpointTex);

	if (checkpoint[1].checkpointTex != nullptr && checkpoint[1].checkpointTex != NULL)
		app->tex->UnLoad(checkpoint[1].checkpointTex);

	for (int i = 0; i < 4; i++)
	{
		if (collectible[i].itemTex != nullptr && collectible[i].itemTex != NULL)
			app->tex->UnLoad(collectible[i].itemTex);
	}

	//OTHER CLEANUPS
	app->enManager->DeleteAllEnemies();

	app->collision->CleanUp();

	app->map->CleanUp();
}

//SETTERS

void Scene::SetScene(Screens scene)
{
	SceneCleanUp();

	previousScreen = currentScreen;
	currentScreen = scene;

	if (scene == TITLE_SCREEN)
		SetTitleScreen();

	else if (scene == MAIN_MENU)
		SetMainMenu();

	else if (scene == DEAD_SCREEN)
		SetDeadScreen();

	else if (scene == LVL1)
		SetLvl1();

	else if (scene == LVL2)
		SetLvl2();

	else if (scene == CONFIG_MENU)
		SetConfigMenu();

	else if (scene == PAUSE_MENU)
		SetPauseMenu();

}

void Scene::SetTitleScreen()
{
	titleScene = app->tex->Load(sourceTitle.GetString());

	timer = 0;
}

void Scene::SetMainMenu()
{
	menuScene = app->tex->Load(sourceIntro.GetString());
}

void Scene::SetLvl1()
{
	app->enManager->player->texture = app->tex->Load(app->enManager->player->textPath.GetString());

	bool checkpointState = checkpoint[0].checked;
	bool coinState = collectible[0].collected;

	//LEVEL  ------------
	app->map->lvl1 = true;
	app->map->lvl2 = false;

	if (app->map->Load(app->map->GetLevelToLoad().GetString()));
	{

		//ACTIVATE ------------
		//ACTIVATE CHECKPOINTS
		checkpoint[0].active = true;
		checkpoint[1].active = false;

		//ACTIVATE LIFE GETTERS
		app->enManager->player->lifeGetter[0].active = true;
		app->enManager->player->lifeGetter[1].active = false;

		//ACTIVATE COLLECTIBLES
		collectible[0].active = true;
		collectible[1].active = true;
		collectible[2].active = false;
		collectible[3].active = false;

		//RESTART ------------
		//RESTART CHECKPOINTS
		checkpoint[0].checked = false;
		checkpoint[1].checked = false;

		//RESTART COLLECTIBLES
		for (int i = 0; i < 4; i++) collectible[i].collected = false;

		//LOADS ------------
		//PLAYER LIFES
		app->enManager->player->playerLife.lifeTex = app->tex->Load(app->enManager->player->playerLife.source.GetString());

		//LIFEGETTER
		app->enManager->player->lifeGetter[0].getterTex = app->tex->Load(app->enManager->player->lifeGetter[0].source.GetString());
		app->enManager->player->lifeGetter[1].getterTex = app->tex->Load(app->enManager->player->lifeGetter[1].source.GetString());

		//SWORD COLLIDER ------------
		app->enManager->player->swordCollider = { -20, 0, 13, 7 };

		// PLAYER INITIALITZATION ------------
		if (checkpointState)
		{
			collectible[0].collected = coinState;

			app->render->camera.x = -2364;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { app->scene->checkpoint[0].rect.x, app->scene->checkpoint[0].rect.y - 16 };
			}
		}
		else
		{
			app->render->camera.x = 0;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { app->map->GetPlayerInitialPos() };
				app->enManager->player->playerInfo.position.y += 2;
			}
		}

		if (app->IsLoading() == false)
		{
			app->enManager->player->playerColider = { app->enManager->player->playerInfo.position.x + 2, app->enManager->player->playerInfo.position.y, 14, 30 };
			app->enManager->player->playerInfo.speed = 2;
			app->enManager->player->playerInfo.currentDir = RIGHT_DIR;
		}

		app->enManager->player->playerInfo.currentAnimation = &app->enManager->player->playerInfo.idle;
	}

	//LOAD ENEMIES
	ListItem<SDL_Rect>* item = app->collision->initPosEnemyGround.start;
	while (item != NULL)
	{
		iPoint pos = { item->data.x, item->data.y };
		app->enManager->CreateEnemyNormal(pos);
		item = item->next;
	}

	ListItem<SDL_Rect>* item2 = app->collision->initPosEnemyFly.start;
	while (item2 != NULL)
	{
		iPoint pos = { item2->data.x, item2->data.y };
		app->enManager->CreateEnemyFly(pos);
		item2 = item2->next;
	}

	//LOAD TEXTURES CP & COLLECTIBLES
	//COLLECTABLE MARKER

	markerTex = app->tex->Load(sourceMarker.GetString());

	//CP
	checkpoint[0].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());
	checkpoint[1].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		collectible[i].itemTex = app->tex->Load(collectible[i].source.GetString());
	}
}

void Scene::SetLvl2()
{
	app->enManager->player->texture = app->tex->Load(app->enManager->player->textPath.GetString());

	bool checkpointState = checkpoint[1].checked;
	bool coinState = collectible[2].collected;

	//LEVEL  ------------
	app->map->lvl1 = false;
	app->map->lvl2 = true;

	if (app->map->Load(app->map->GetLevelToLoad().GetString()));
	{

		//ACTIVATE ------------
		//ACTIVATE CHECKPOINTS
		checkpoint[0].active = false;
		checkpoint[1].active = true;

		//ACTIVATE LIFE GETTERS
		app->enManager->player->lifeGetter[0].active = false;
		app->enManager->player->lifeGetter[1].active = true;

		//ACTIVATE COLLECTIBLES
		collectible[0].active = false;
		collectible[1].active = false;
		collectible[2].active = true;
		collectible[3].active = true;

		//RESTART ------------
		//RESTART CHECKPOINTS
		checkpoint[0].checked = false;
		checkpoint[1].checked = false;

		//RESTART COLLECTIBLES
		for (int i = 0; i < 4; i++) collectible[i].collected = false;

		//LOADS ------------
		//PLAYER LIFES
		app->enManager->player->playerLife.lifeTex = app->tex->Load(app->enManager->player->playerLife.source.GetString());

		//LIFEGETTER
		app->enManager->player->lifeGetter[0].getterTex = app->tex->Load(app->enManager->player->lifeGetter[0].source.GetString());
		app->enManager->player->lifeGetter[1].getterTex = app->tex->Load(app->enManager->player->lifeGetter[1].source.GetString());

		//SWORD COLLIDER ------------
		app->enManager->player->swordCollider = { -20, 0, 13, 7 };

		// PLAYER INITIALITZATION ------------
		if (checkpointState)
		{
			collectible[2].collected = coinState;

			app->render->camera.x = -765;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { checkpoint[1].rect.x, checkpoint[1].rect.y - 16 };
			}
		}
		else
		{
			app->render->camera.x = 0;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { app->map->GetPlayerInitialPos() };
				app->enManager->player->playerInfo.position.y += 2;
			}
		}

		if (app->IsLoading() == false)
		{
			app->enManager->player->playerColider = { app->enManager->player->playerInfo.position.x + 2, app->enManager->player->playerInfo.position.y, 14, 30 };
			app->enManager->player->playerInfo.speed = 2;
			app->enManager->player->playerInfo.currentDir = RIGHT_DIR;
		}

		app->enManager->player->playerInfo.currentAnimation = &app->enManager->player->playerInfo.idle;
	}

	//LOAD TEXTURES CP & COLLECTIBLES
	//COLLECTABLE MARKER

	markerTex = app->tex->Load(sourceMarker.GetString());

	//CP
	checkpoint[0].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());
	checkpoint[1].checkpointTex = app->tex->Load(checkpoint[0].source.GetString());

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		collectible[i].itemTex = app->tex->Load(collectible[i].source.GetString());
	}
}

void Scene::SetDeadScreen()
{
	app->render->camera.x = 0;

	deathScene = app->tex->Load(sourceDeath.GetString());

	app->enManager->player->Dead();
}

void Scene::SetConfigMenu()
{

}

void Scene::SetPauseMenu()
{

}

//UPDATERS

void Scene::UpdateTitle()
{
	app->render->DrawTexture(titleScene, 0, 0);

	if (timer < 200)
	{
		timer++;
	}
	else if (timer >= 200 && timer < 230)
	{
		app->fade->FadeEffect(false, 30.0f);
		timer++;
	}
	else
	{
		SetScene(MAIN_MENU);
	}
}

void Scene::UpdateMainMenu()
{
	app->render->DrawTexture(menuScene, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		SetScene(LVL1);
	}
}

void Scene::UpdateLevels()
{
	// LOGIC --------------------------
	if (checkpoint[0].active && app->collision->CheckCollision(app->enManager->player->playerColider, checkpoint[0].rect))
	{
		if (checkpoint[0].checked == false)
		{
			app->audio->PlayFx(2);
		}
		checkpoint[0].checked = true;
	}
	else if (checkpoint[1].active && app->collision->CheckCollision(app->enManager->player->playerColider, checkpoint[1].rect))
	{
		if (checkpoint[1].checked == false)
		{
			app->audio->PlayFx(2);
		}
		checkpoint[1].checked = true;
	}

	//LIFE GETTERS
	for (int i = 0; i < 2; i++)
	{
		if (app->enManager->player->lifeGetter[i].active && app->collision->CheckCollision(app->enManager->player->playerColider, app->enManager->player->lifeGetter[i].getterRect))
		{
			app->enManager->player->playerLife.lifes = app->enManager->player->lifeGetter[i].refill;
		}
	}

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		if (collectible[i].active && app->collision->CheckCollision(app->enManager->player->playerColider, collectible[i].itemRect))
		{
			if (collectible[i].collected == false)
			{
				app->audio->PlayFx(3);
			}
			collectible[i].collected = true;
		}
	}
	// --------------------------------

	// DRAW ---------------------------
	// MAP
	app->map->Draw();

	// LIFEGETTERS DRAW
	if (app->enManager->player->lifeGetter[0].active)
	{
		app->render->DrawTexture(app->enManager->player->lifeGetter[0].getterTex, app->enManager->player->lifeGetter[0].getterRect.x, app->enManager->player->lifeGetter[0].getterRect.y);
	}
	else if (app->enManager->player->lifeGetter[1].active)
	{
		app->render->DrawTexture(app->enManager->player->lifeGetter[1].getterTex, app->enManager->player->lifeGetter[1].getterRect.x, app->enManager->player->lifeGetter[1].getterRect.y);
	}

	// PLAYER
	app->enManager->player->Draw();
	// --------------------------------
	
	// ANIMATION ---------------------
	//IDLE ANIMATION
	if (strcmp(app->enManager->player->playerInfo.currentAnimation->name.GetString(), "idle") != 0 || strcmp(app->enManager->player->playerInfo.currentAnimation->name.GetString(), "idleLeft") != 0)
	{
		if (!app->enManager->player->playerInfo.currentAnimation->Finished())
		{
			app->enManager->player->playerInfo.currentAnimation->FinishAnimation();
			app->enManager->player->UpdateAnimation("idle");
			app->enManager->player->isMoving = false;
		}
	}
	// -------------------------------

	// DEBUG  ---------------------------
	// GO TO LEVEL 1 - F1
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		checkpoint[0].checked = false;
		SetScene(LVL1);
	}

	// GO TO LEVEL 2 - F2
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		checkpoint[1].checked = false;
		SetScene(LVL2);
	}

	// RESTART ACTUAL LEVEL - F3
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		if (currentScreen == LVL1)
		{
			checkpoint[0].checked = false;
			SetScene(LVL1);
		}

		else if (currentScreen == LVL2)
		{
			checkpoint[1].checked = false;
			SetScene(LVL2);
		}
	}

	// RESTART OTHER LEVEL - F4
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		if (currentScreen == LVL1)
		{
			checkpoint[1].checked = false;
			SetScene(LVL2);
		}

		else if (currentScreen == LVL2)
		{
			checkpoint[0].checked = false;
			SetScene(LVL1);
		}
	}

	// GO TO CHECKPOINT 1
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		checkpoint[0].checked = true;
		SetScene(LVL1);
	}

	// GO TO CHECKPOINT 2
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		checkpoint[1].checked = true;
		SetScene(LVL2);
	}

	// HURT PLAYER
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (app->enManager->player->playerLife.lifes > 0) app->enManager->player->playerLife.lifes--;
	}

	// SAVE & LOAD
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveRequest("save_game.xml");
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadRequest("save_game.xml");

	// VOLUME SETTING
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		app->audio->SetVolume(0);

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		app->audio->SetVolume(1);

	//LIFE, CP & COLLECTIBLES
	//LIFES
		if (app->enManager->player->playerLife.lifes == 3)
		{
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 50 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 70 - app->render->camera.x / 3, 3);
		}
		else if (app->enManager->player->playerLife.lifes == 2)
		{
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 50 - app->render->camera.x / 3, 3);
		}
		else if (app->enManager->player->playerLife.lifes == 1)
		{
			app->render->DrawTexture(app->enManager->player->playerLife.lifeTex, 30 - app->render->camera.x / 3, 3);
		}

		if (app->enManager->player->playerLife.lifes == 0)
		{
			app->enManager->player->playerLife.lifes = 3;
			SetScene(DEAD_SCREEN);
		}

		//CP
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

		//COLLECTIBLES
		for (int i = 0; i < 4; i++)
		{
			if (collectible[i].active && !collectible[i].collected)
			{
				app->render->DrawTexture(collectible[i].itemTex, collectible[i].itemRect.x, collectible[i].itemRect.y);
			}
		}

		//COLLECTIBLES MARKER LOGIC
		CollectibleMarkerLogic();

		//WIN
		if (app->enManager->player->CheckWin() == true)
		{
			if (currentScreen == LVL1)
			{
				checkpoint[1].checked = false;
				SetScene(LVL2);
			}

			else if (currentScreen == LVL2)
			{
				checkpoint[0].checked = false;
				SetScene(LVL1);
			}
		}
}

void Scene::UpdateDeadScreen()
{
	app->render->DrawTexture(deathScene, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		SetScene(previousScreen);
	}
}

void Scene::UpdateConfigMenu()
{

}

void Scene::UpdatePauseMenu()
{

}
