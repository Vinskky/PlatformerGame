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
#include "Entity.h"
#include "GuiManager.h"
#include "GuiString.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

enum EntityType;

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
	sceneConf = conf;

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
	return !exit;
}

bool Scene::CleanUp()
{
	return true;
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

	if (creditsMenu != nullptr && creditsMenu != NULL)
		app->tex->UnLoad(creditsMenu);

	//GUI UNLOAD
	if (app->enManager->player->playerLife.lifeTex != nullptr && app->enManager->player->playerLife.lifeTex != NULL)
		app->tex->UnLoad(app->enManager->player->playerLife.lifeTex);

	if (app->enManager->props->markerTex != nullptr && app->enManager->props->markerTex != NULL)
		app->tex->UnLoad(app->enManager->props->markerTex);

	//ENTITIES UNLOAD
	if (app->enManager->props->lifeGetter[0].getterTex != nullptr && app->enManager->props->lifeGetter[0].getterTex != NULL)
		app->tex->UnLoad(app->enManager->props->lifeGetter[0].getterTex);

	if (app->enManager->props->lifeGetter[1].getterTex != nullptr && app->enManager->props->lifeGetter[1].getterTex != NULL)
		app->tex->UnLoad(app->enManager->props->lifeGetter[1].getterTex);

	if (app->enManager->player->texture != nullptr && app->enManager->player->texture != NULL)
		app->tex->UnLoad(app->enManager->player->texture);

	if (app->enManager->props->checkpoint[0].checkpointTex != nullptr && app->enManager->props->checkpoint[0].checkpointTex != NULL)
		app->tex->UnLoad(app->enManager->props->checkpoint[0].checkpointTex);

	if (app->enManager->props->checkpoint[1].checkpointTex != nullptr && app->enManager->props->checkpoint[1].checkpointTex != NULL)
		app->tex->UnLoad(app->enManager->props->checkpoint[1].checkpointTex);

	for (int i = 0; i < 4; i++)
	{
		if (app->enManager->props->collectible[i].itemTex != nullptr && app->enManager->props->collectible[i].itemTex != NULL)
			app->tex->UnLoad(app->enManager->props->collectible[i].itemTex);
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

	creditsMenu = app->tex->Load("Assets/textures/credits.png");

	configMenu = app->tex->Load("Assets/textures/config_menu.png");

	//PLAY BUTTON
	if (playButton == NULL && playButton == nullptr)
	{
		playButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		playButton->bounds = { 164, 122 - 60, 105, 27 };
		playButton->id = 1;
		playButton->text = "PlayButton";
		playButton->SetObserver(this);

		strPlay = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strPlay->bounds = { playButton->bounds.x + 27, playButton->bounds.y + 3, 105, 27 };
		strPlay->SetString("Play");
	}

	//CONTINUE BUTTON
	if (continueButton == NULL && continueButton == nullptr)
	{
		continueButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		continueButton->bounds = { 164, 155 - 60, 105, 27 };
		continueButton->id = 2;
		continueButton->text = "ContinueButton";
		continueButton->SetObserver(this);
		continueButton->state = GuiControlState::DISABLED;
		strContinue = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strContinue->bounds = { continueButton->bounds.x + 8, continueButton->bounds.y + 3, 105, 27 };
		strContinue->SetString("Continue");
	}

	//CONFIG BUTTON
	if (configButton == NULL && configButton == nullptr)
	{
		configButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		configButton->bounds = { 164, 188 - 60, 105, 27 };
		configButton->id = 2;
		configButton->text = "ConfigButton";
		configButton->SetObserver(this);
		strConfig = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strConfig->bounds = { configButton->bounds.x + 10, configButton->bounds.y + 3, 105, 27 };
		strConfig->SetString("Settings");
	}

	//CREDITS BUTTON
	if (creditsButton == NULL && creditsButton == nullptr)
	{
		creditsButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		creditsButton->bounds = { 164, 221 - 60, 105, 27 };
		creditsButton->id = 2;
		creditsButton->text = "CreditsButton";
		creditsButton->SetObserver(this);
		strCredits = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strCredits->bounds = { creditsButton->bounds.x + 15, creditsButton->bounds.y + 3, 105, 27 };
		strCredits->SetString("Credits");
	}

	//EXITGAME BUTTON
	if (exitMainButton == NULL && exitMainButton == nullptr)
	{
		exitMainButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		exitMainButton->bounds = { 164, 254 - 60, 105, 27 };
		exitMainButton->id = 4;
		exitMainButton->text = "ExitButton";
		exitMainButton->SetObserver(this);
		strExit = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strExit->bounds = { exitMainButton->bounds.x + 32, exitMainButton->bounds.y + 3, 105, 27 };
		strExit->SetString("Exit");
	}

}

void Scene::SetLvl1()
{
	app->enManager->player->texture = app->tex->Load(app->enManager->player->textPath.GetString());

	bool checkpointState = app->enManager->props->checkpoint[0].checked;
	bool coinState = app->enManager->props->collectible[0].collected;

	//LEVEL  ------------
	app->map->lvl1 = true;
	app->map->lvl2 = false;

	if (app->map->Load(app->map->GetLevelToLoad().GetString()));
	{

		//ACTIVATE ------------
		//ACTIVATE CHECKPOINTS
		app->enManager->props->checkpoint[0].active = true;
		app->enManager->props->checkpoint[1].active = false;

		//ACTIVATE LIFE GETTERS
		app->enManager->props->lifeGetter[0].active = true;
		app->enManager->props->lifeGetter[1].active = false;

		//ACTIVATE COLLECTIBLES
		app->enManager->props->collectible[0].active = true;
		app->enManager->props->collectible[1].active = true;
		app->enManager->props->collectible[2].active = false;
		app->enManager->props->collectible[3].active = false;

		//RESTART ------------
		//RESTART CHECKPOINTS
		app->enManager->props->checkpoint[0].checked = false;
		app->enManager->props->checkpoint[1].checked = false;

		//RESTART COLLECTIBLES
		for (int i = 0; i < 4; i++) app->enManager->props->collectible[i].collected = false;

		//LOADS ------------
		//PLAYER LIFES
		app->enManager->player->playerLife.lifeTex = app->tex->Load(app->enManager->player->playerLife.source.GetString());

		//LIFEGETTER
		app->enManager->props->lifeGetter[0].getterTex = app->tex->Load(app->enManager->props->lifeGetter[0].source.GetString());
		app->enManager->props->lifeGetter[1].getterTex = app->tex->Load(app->enManager->props->lifeGetter[1].source.GetString());

		//SWORD COLLIDER ------------
		app->enManager->player->swordCollider = { -20, 0, 13, 7 };

		// PLAYER INITIALITZATION ------------
		if (checkpointState)
		{
			app->enManager->props->collectible[0].collected = coinState;

			app->render->camera.x = -2364;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { app->enManager->props->checkpoint[0].rect.x, app->enManager->props->checkpoint[0].rect.y - 16 };
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

	app->enManager->props->markerTex = app->tex->Load(app->enManager->props->sourceMarker.GetString());

	//CP
	app->enManager->props->checkpoint[0].checkpointTex = app->tex->Load(app->enManager->props->checkpoint[0].source.GetString());
	app->enManager->props->checkpoint[1].checkpointTex = app->tex->Load(app->enManager->props->checkpoint[1].source.GetString());

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		app->enManager->props->collectible[i].itemTex = app->tex->Load(app->enManager->props->collectible[i].source.GetString());
	}
}

void Scene::SetLvl2()
{
	app->enManager->player->texture = app->tex->Load(app->enManager->player->textPath.GetString());

	bool checkpointState = app->enManager->props->checkpoint[1].checked;
	bool coinState = app->enManager->props->collectible[2].collected;

	//LEVEL  ------------
	app->map->lvl1 = false;
	app->map->lvl2 = true;

	if (app->map->Load(app->map->GetLevelToLoad().GetString()));
	{

		//ACTIVATE ------------
		//ACTIVATE CHECKPOINTS
		app->enManager->props->checkpoint[0].active = false;
		app->enManager->props->checkpoint[1].active = true;

		//ACTIVATE LIFE GETTERS
		app->enManager->props->lifeGetter[0].active = false;
		app->enManager->props->lifeGetter[1].active = true;

		//ACTIVATE COLLECTIBLES
		app->enManager->props->collectible[0].active = false;
		app->enManager->props->collectible[1].active = false;
		app->enManager->props->collectible[2].active = true;
		app->enManager->props->collectible[3].active = true;

		//RESTART ------------
		//RESTART CHECKPOINTS
		app->enManager->props->checkpoint[0].checked = false;
		app->enManager->props->checkpoint[1].checked = false;

		//RESTART COLLECTIBLES
		for (int i = 0; i < 4; i++) app->enManager->props->collectible[i].collected = false;

		//LOADS ------------
		//PLAYER LIFES
		app->enManager->player->playerLife.lifeTex = app->tex->Load(app->enManager->player->playerLife.source.GetString());

		//LIFEGETTER
		app->enManager->props->lifeGetter[0].getterTex = app->tex->Load(app->enManager->props->lifeGetter[0].source.GetString());
		app->enManager->props->lifeGetter[1].getterTex = app->tex->Load(app->enManager->props->lifeGetter[1].source.GetString());

		//SWORD COLLIDER ------------
		app->enManager->player->swordCollider = { -20, 0, 13, 7 };

		// PLAYER INITIALITZATION ------------
		if (checkpointState)
		{
			app->enManager->props->collectible[2].collected = coinState;

			app->render->camera.x = -765;

			if (app->IsLoading() == false)
			{
				app->enManager->player->playerInfo.position = { app->enManager->props->checkpoint[1].rect.x, app->enManager->props->checkpoint[1].rect.y - 16 };
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

	app->enManager->props->markerTex = app->tex->Load(app->enManager->props->sourceMarker.GetString());

	//CP
	app->enManager->props->checkpoint[0].checkpointTex = app->tex->Load(app->enManager->props->checkpoint[0].source.GetString());
	app->enManager->props->checkpoint[1].checkpointTex = app->tex->Load(app->enManager->props->checkpoint[1].source.GetString());

	//COLLECTIBLES
	for (int i = 0; i < 4; i++)
	{
		app->enManager->props->collectible[i].itemTex = app->tex->Load(app->enManager->props->collectible[i].source.GetString());
	}
}

void Scene::SetDeadScreen()
{
	//app->guiManager->buttonSpritesheet = app->tex->Load("Assets/textures/button_spritesheet.png");

	app->render->camera.x = 0;

	deathScene = app->tex->Load(sourceDeath.GetString());

	app->enManager->player->Dead();
}

void Scene::SetConfigMenu()
{
	if (currentScreen != MAIN_MENU) configMenu = app->tex->Load("Assets/textures/config_menu.png");

	//SLIDER VOLUME
	if (sliderVolume == NULL && sliderVolume == nullptr)
	{
		sliderVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
		sliderVolume->bounds = { 194, 70, 110, 10 };
		sliderVolume->text = "VolumeSlider";
		sliderVolume->SetMinValue(0);
		sliderVolume->SetMaxValue(124);
		sliderVolume->SetSlider(sliderVolume->bounds);
		sliderVolume->SetObserver(this);
		strVolume = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strVolume->bounds = { sliderVolume->bounds.x - 100, sliderVolume->bounds.y, 105, 27 };
		strVolume->SetString("Volume");
	}
	//SLIDER FX
	if (sliderFX == NULL && sliderFX == nullptr)
	{
		sliderFX = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
		sliderFX->bounds = { 194, 40, 110, 10 };
		sliderFX->text = "FXSlider";
		sliderFX->SetMinValue(0);
		sliderFX->SetMaxValue(124);
		sliderFX->SetSlider(sliderFX->bounds);
		sliderFX->SetObserver(this);
		strFX = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strFX->bounds = { sliderFX->bounds.x - 100, sliderFX->bounds.y, 105, 27 };
		strFX->SetString("FX");
	}
	//BACK TO PAUSE BUTTON
	if (backToPauseButton == NULL && backToPauseButton == nullptr)
	{
		backToPauseButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		backToPauseButton->bounds = { 164, 200, 105, 27 };
		backToPauseButton->id = 1;
		backToPauseButton->text = "BackToButton";
		backToPauseButton->SetObserver(this);
		strBackToPause = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strBackToPause->bounds = { backToPauseButton->bounds.x + 30, backToPauseButton->bounds.y, 105, 27 };
		strBackToPause->SetString("Back");
	}

	//VSYNC CHECKBOX
	if (vSyncCheckBox == NULL && vSyncCheckBox == nullptr)
	{
		vSyncCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl((GuiControlType)GuiControlType::CHECKBOX);
		vSyncCheckBox->bounds = { 70, 100, 25, 25 };
		vSyncCheckBox->id = 1;
		vSyncCheckBox->text = "VSyncCheckBox";
		vSyncCheckBox->SetObserver(this);
		strVsync = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strVsync->bounds = { vSyncCheckBox->bounds.x + 30, vSyncCheckBox->bounds.y + 2, 0, 0 };
		strVsync->SetString("Vsync");
	}

	if (fullScreenCheckBox == NULL && fullScreenCheckBox == nullptr)
	{
		fullScreenCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl((GuiControlType)GuiControlType::CHECKBOX);
		fullScreenCheckBox->bounds = { 70, 130, 25, 25 };
		fullScreenCheckBox->id = 2;
		fullScreenCheckBox->text = "FullScreenCheckBox";
		fullScreenCheckBox->SetObserver(this);
		strFullScreen = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strFullScreen->bounds = { fullScreenCheckBox->bounds.x + 30, fullScreenCheckBox->bounds.y + 2, 0, 0 };
		strFullScreen->SetString("FullScreen");
	}
}

void Scene::SetPauseMenu()
{
	previousScreen = currentScreen;
	currentScreen = PAUSE_MENU;
	pausePlayerPosition = app->enManager->player->playerInfo.position;

	for (int i = 0; i < app->enManager->entities.Count(); i++)
	{
		if (app->enManager->entities.At(i)->data->type == (EntityType)0)
		{
			pauseEnemyPosition = app->enManager->entities.At(i)->data->pos;
		}
	}

	pauseCamPositionX = app->render->camera.x;
	app->render->camera.x = 0;

	app->tex->UnLoad(pauseMenu);
	pauseMenu = app->tex->Load("Assets/textures/pause_menu.png");

	//RESUME BUTTON
	if (resumeButton == NULL && resumeButton == nullptr)
	{
		resumeButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		resumeButton->bounds = { 164, 122 - 25, 105, 27 };
		resumeButton->id = 1;
		resumeButton->text = "ResumeButton";
		resumeButton->SetObserver(this);
		strResume = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strResume->bounds = { resumeButton->bounds.x + 15, resumeButton->bounds.y, 0,0 };
		strResume->SetString("Resume");
	}

	//CONFIG BUTTON
	if (configPauseButton == NULL && configPauseButton == nullptr)
	{
		configPauseButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		configPauseButton->bounds = { 164, 155 - 25, 105, 27 };
		configPauseButton->id = 2;
		configPauseButton->text = "ConfigButton";
		configPauseButton->SetObserver(this);
		strConfigPause = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strConfigPause->bounds = { configPauseButton->bounds.x + 10, configPauseButton->bounds.y, 0,0 };
		strConfigPause->SetString("Settings");
	}

	//BACK TO MENU BUTTON
	if (mainMenuPauseButton == NULL && mainMenuPauseButton == nullptr)
	{
		mainMenuPauseButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		mainMenuPauseButton->bounds = { 164, 188 - 25, 105, 27 };
		mainMenuPauseButton->id = 3;
		mainMenuPauseButton->text = "BackToMainButton";
		mainMenuPauseButton->SetObserver(this);
		strMainMenu = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strMainMenu->bounds = { mainMenuPauseButton->bounds.x + 2, mainMenuPauseButton->bounds.y, 0,0 };
		strMainMenu->SetString("Main Menu");
	}

	//EXITGAME BUTTON
	if (exitPauseButton == NULL && exitPauseButton == nullptr)
	{
		exitPauseButton = (GuiButton*)app->guiManager->CreateGuiControl((GuiControlType)0);
		exitPauseButton->bounds = { 164, 221 - 25, 105, 27 };
		exitPauseButton->id = 4;
		exitPauseButton->text = "ExitButton";
		exitPauseButton->SetObserver(this);
		strExitPause = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strExitPause->bounds = { exitPauseButton->bounds.x + 5, exitPauseButton->bounds.y, 0,0 };
		strExitPause->SetString("Exit Game");
	}
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
	if (!config)
	{
		if (!credits)
		{
			app->render->DrawTexture(menuScene, 0, 0);

			//UPDATE BUTTONS
			playButton->Update(1.0f);
			continueButton->Update(1.0f);
			configButton->Update(1.0f);
			creditsButton->Update(1.0f);
			exitMainButton->Update(1.0f);

			//DRAW BUTTONS && TEXT
			playButton->Draw();
			strPlay->Draw();
			continueButton->Draw();
			if (continueButton->state != GuiControlState::DISABLED) strContinue->Draw();
			configButton->Draw();
			strConfig->Draw();
			creditsButton->Draw();
			strCredits->Draw();
			exitMainButton->Draw();
			strExit->Draw();
		}
		else if (credits)
		{
			app->render->DrawTexture(configMenu, 0, 0);

			//DRAW BUTTONS
			/*playButton->Draw();
			continueButton->Draw();
			configButton->Draw();
			creditsButton->Draw();
			exitMainButton->Draw();*/

			app->render->DrawTexture(creditsMenu, 142, 47);

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) credits = false;
		}
	}
	else if (config)
	{
		UpdateConfigMenu();
	}
}

void Scene::UpdateLevels()
{
	if (!pause)
	{
		// LOGIC --------------------------
		if (app->enManager->props->checkpoint[0].active && app->collision->CheckCollision(app->enManager->player->playerColider, app->enManager->props->checkpoint[0].rect))
		{
			if (app->enManager->props->checkpoint[0].checked == false)
			{
				app->audio->PlayFx(2);
			}
			app->enManager->props->checkpoint[0].checked = true;
		}
		else if (app->enManager->props->checkpoint[1].active && app->collision->CheckCollision(app->enManager->player->playerColider, app->enManager->props->checkpoint[1].rect))
		{
			if (app->enManager->props->checkpoint[1].checked == false)
			{
				app->audio->PlayFx(2);
			}
			app->enManager->props->checkpoint[1].checked = true;
		}

		//LIFE GETTERS
		for (int i = 0; i < 2; i++)
		{
			if (app->enManager->props->lifeGetter[i].active && app->collision->CheckCollision(app->enManager->player->playerColider, app->enManager->props->lifeGetter[i].getterRect))
			{
				app->enManager->player->playerLife.lifes = app->enManager->props->lifeGetter[i].refill;
			}
		}

		//COLLECTIBLES
		for (int i = 0; i < 4; i++)
		{
			if (app->enManager->props->collectible[i].active && app->collision->CheckCollision(app->enManager->player->playerColider, app->enManager->props->collectible[i].itemRect))
			{
				if (app->enManager->props->collectible[i].collected == false)
				{
					app->audio->PlayFx(3);
				}
				app->enManager->props->collectible[i].collected = true;
			}
		}
		// --------------------------------

		// DRAW ---------------------------
		// MAP
		app->map->Draw();

		// LIFEGETTERS DRAW
		if (app->enManager->props->lifeGetter[0].active)
		{
			app->render->DrawTexture(app->enManager->props->lifeGetter[0].getterTex, app->enManager->props->lifeGetter[0].getterRect.x, app->enManager->props->lifeGetter[0].getterRect.y);
		}
		else if (app->enManager->props->lifeGetter[1].active)
		{
			app->render->DrawTexture(app->enManager->props->lifeGetter[1].getterTex, app->enManager->props->lifeGetter[1].getterRect.x, app->enManager->props->lifeGetter[1].getterRect.y);
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
			app->enManager->props->checkpoint[0].checked = false;
			SetScene(LVL1);
		}

		// GO TO LEVEL 2 - F2
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			app->enManager->props->checkpoint[1].checked = false;
			SetScene(LVL2);
		}

		// RESTART ACTUAL LEVEL - F3
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			if (currentScreen == LVL1)
			{
				app->enManager->props->checkpoint[0].checked = false;
				SetScene(LVL1);
			}

			else if (currentScreen == LVL2)
			{
				app->enManager->props->checkpoint[1].checked = false;
				SetScene(LVL2);
			}
		}

		// RESTART OTHER LEVEL - F4
		if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			if (currentScreen == LVL1)
			{
				app->enManager->props->checkpoint[1].checked = false;
				SetScene(LVL2);
			}

			else if (currentScreen == LVL2)
			{
				app->enManager->props->checkpoint[0].checked = false;
				SetScene(LVL1);
			}
		}

		// GO TO CHECKPOINT 1
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			app->enManager->props->checkpoint[0].checked = true;
			SetScene(LVL1);
		}

		// GO TO CHECKPOINT 2
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			app->enManager->props->checkpoint[1].checked = true;
			SetScene(LVL2);
		}

		// HURT PLAYER
		if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			if (app->enManager->player->playerLife.lifes > 0) app->enManager->player->playerLife.lifes--;
		}

		// SAVE & LOAD
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveRequest("save_game.xml");
			continueButton->state = GuiControlState::NORMAL;
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadRequest("save_game.xml");

		// VOLUME SETTING
		if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
			app->audio->SetVolume(0);

		if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
			app->audio->SetVolume(1);

		// PAUSE GAME
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			SetPauseMenu();
			pause = true;
		}

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

		if (app->enManager->props->checkpoint[0].active && !app->enManager->props->checkpoint[0].checked && currentScreen != DEAD_SCREEN)
		{
			app->render->DrawTexture(app->enManager->props->checkpoint[0].checkpointTex, app->enManager->props->checkpoint[0].rect.x, app->enManager->props->checkpoint[0].rect.y, &sectionCPAnim1);
		}
		else if (app->enManager->props->checkpoint[0].active && app->enManager->props->checkpoint[0].checked && currentScreen != DEAD_SCREEN)
		{
			app->render->DrawTexture(app->enManager->props->checkpoint[0].checkpointTex, app->enManager->props->checkpoint[0].rect.x, app->enManager->props->checkpoint[0].rect.y, &sectionCPAnim2);
		}

		if (app->enManager->props->checkpoint[1].active && !app->enManager->props->checkpoint[1].checked && currentScreen != DEAD_SCREEN)
		{
			app->render->DrawTexture(app->enManager->props->checkpoint[1].checkpointTex, app->enManager->props->checkpoint[1].rect.x, app->enManager->props->checkpoint[1].rect.y, &sectionCPAnim1);
		}
		else if (app->enManager->props->checkpoint[1].active && app->enManager->props->checkpoint[1].checked && currentScreen != DEAD_SCREEN)
		{
			app->render->DrawTexture(app->enManager->props->checkpoint[1].checkpointTex, app->enManager->props->checkpoint[1].rect.x, app->enManager->props->checkpoint[1].rect.y, &sectionCPAnim2);
		}

		//COLLECTIBLES
		for (int i = 0; i < 4; i++)
		{
			if (app->enManager->props->collectible[i].active && !app->enManager->props->collectible[i].collected)
			{
				app->render->DrawTexture(app->enManager->props->collectible[i].itemTex, app->enManager->props->collectible[i].itemRect.x, app->enManager->props->collectible[i].itemRect.y);
			}
		}

		//COLLECTIBLES MARKER LOGIC
		app->enManager->props->CollectibleMarkerLogic();

		//WIN
		if (app->enManager->player->CheckWin() == true)
		{
			if (currentScreen == LVL1)
			{
				app->enManager->props->checkpoint[1].checked = false;
				SetScene(LVL2);
			}

			else if (currentScreen == LVL2)
			{
				app->enManager->props->checkpoint[0].checked = false;
				SetScene(LVL1);
			}
		}
	}
	else if (pause)
	{
		UpdatePauseMenu();
	}
}

void Scene::UpdateDeadScreen()
{
	app->render->DrawTexture(deathScene, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		SetScene(MAIN_MENU);
	}
}

void Scene::UpdateConfigMenu()
{
	app->render->DrawTexture(configMenu, 0, 0);
	

	sliderVolume->Update(0.0f);
	sliderVolume->Draw();
	strVolume->Draw();

	sliderFX->Update(0.0f);
	sliderFX->Draw();
	strFX->Draw();

	backToPauseButton->Update(0.0f);
	backToPauseButton->Draw();
	strBackToPause->Draw();

	vSyncCheckBox->Update(0.0f);
	vSyncCheckBox->Draw();
	strVsync->Draw();

	fullScreenCheckBox->Update(0.0f);
	fullScreenCheckBox->Draw();
	strFullScreen->Draw();
}

void Scene::UpdatePauseMenu()
{
	if (!config)
	{
		app->render->DrawTexture(pauseMenu, 0, 0);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			pause = false;
		}

		app->enManager->player->playerInfo.position = pausePlayerPosition;

		for (int i = 0; i < app->enManager->entities.Count(); i++)
		{
			if (app->enManager->entities.At(i)->data->type == (EntityType)0)
			{
				app->enManager->entities.At(i)->data->pos = pauseEnemyPosition;
			}
		}

		resumeButton->Update(1.0f);
		configPauseButton->Update(1.0f);
		mainMenuPauseButton->Update(1.0f);
		exitPauseButton->Update(1.0f);

		resumeButton->Draw();
		strResume->Draw();
		configPauseButton->Draw();
		strConfigPause->Draw();
		mainMenuPauseButton->Draw();
		strMainMenu->Draw();
		exitPauseButton->Draw();
		strExitPause->Draw();
	}
	else if (config)
	{
		UpdateConfigMenu();
	}
}

//CLICK EVENT

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currentScreen)
	{
	case MAIN_MENU:

		if (!config)
		{
			if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "PlayButton") == 0)
			{
				SetScene(LVL1);
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ContinueButton") == 0)
			{
				SetScene(LVL1);
				app->LoadRequest("save_game.xml");
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ConfigButton") == 0)
			{
				SetConfigMenu();
				config = true;
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "CreditsButton") == 0)
			{
				credits = true;
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ExitButton") == 0)
			{
				exit = true;
			}
		}
		else if (config)
		{

			if (control->type == (GuiControlType)GuiControlType::BUTTON)
			{
				if (strcmp(control->text.GetString(), "BackToButton") == 0) config = false;
			}
			else if (control->type == (GuiControlType)GuiControlType::CHECKBOX)
			{
				if (strcmp(control->text.GetString(), "VSyncCheckBox") == 0)
				{
					if (vSyncCheckBox->GetCheckedState())
					{
						app->render->SetVSync(true);
					}
					else if (!vSyncCheckBox->GetCheckedState())
					{
						app->render->SetVSync(false);
					}
				}
				else if (strcmp(control->text.GetString(), "FullScreenCheckBox") == 0)
				{
					if (fullScreenCheckBox->GetCheckedState()) 
					{ 
						app->win->SetWinFullScreen(true); 
					}
					else if (!fullScreenCheckBox->GetCheckedState())
					{
						app->win->SetWinFullScreen(false);
					}
				}
			}
		}

		break;

	case PAUSE_MENU:

		if (!config)
		{
			if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ResumeButton") == 0)
			{
				pause = false;
				currentScreen = previousScreen;
				app->render->camera.x = pauseCamPositionX;
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ConfigButton") == 0)
			{
				SetConfigMenu();
				config = true;
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "BackToMainButton") == 0)
			{
				SetScene(MAIN_MENU);
				pause = false;
			}
			else if (control->type == (GuiControlType)0 && strcmp(control->text.GetString(), "ExitButton") == 0)
			{
				exit = true;
			}
		}
		else if (config)
		{
			if (control->type == (GuiControlType)GuiControlType::BUTTON)
			{
				if (strcmp(control->text.GetString(), "BackToButton") == 0) config = false;
			}
			else if (control->type == (GuiControlType)GuiControlType::CHECKBOX)
			{
				if (strcmp(control->text.GetString(), "VSyncCheckBox") == 0)
				{
					if (vSyncCheckBox->GetCheckedState()) int bla;
					else if (!vSyncCheckBox->GetCheckedState()) int bla;
				}
				else if (strcmp(control->text.GetString(), "FullScreenCheckBox") == 0)
				{
					if (fullScreenCheckBox->GetCheckedState())
					{
						app->win->SetWinFullScreen(true);
					}
					else if (!fullScreenCheckBox->GetCheckedState())
					{
						app->win->SetWinFullScreen(false);
					}
				}
			}
		}

		break;
	}

	return true;
}
