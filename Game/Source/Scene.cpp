#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "FadeInFadeOut.h"
#include "Map.h"
#include "Collider.h"

#include "Defs.h"
#include "Log.h"

#define MAX_RIGHT_DISTANCE 320
#define MAX_LEFT_DISTANCE 50
const float deltaTime = 1.0f / 60.0f;

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
	sourceTitle = conf.child("title").attribute("name").as_string();
	sourceIntro = conf.child("intro").attribute("name").as_string();
	sourceDeath = conf.child("death").attribute("name").as_string();
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	titleScene = app->tex->Load(sourceTitle.GetString());
	introScene = app->tex->Load(sourceIntro.GetString());
	deathScene = app->tex->Load(sourceDeath.GetString());
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
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
	if (introKey == false && titleKey == true)
	{
		if (app->player->onGround == true) {
			app->player->jumpOn = true;
		}

		if(app->player->onGround == false)
			app->player->Gravity();

		// L02: TODO 3: Request Load / Save when pressing L/S
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			app->player->LoadCurrentLevel(LVL_1);
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			app->player->LoadCurrentLevel(LVL_2);
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			app->player->LoadCurrentLevel(app->player->playerInfo.currentLevel);

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveRequest("savegame.xml");
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadRequest("savegame.xml");
		//L02: BONUS CODE
		if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
			app->audio->SetVolume(0);

		if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
			app->audio->SetVolume(1);

		//PLAYER MOVEMENT + COLLISIONS

		tempPlayerPosition = app->player->playerInfo.position;
		tempRectPlayer = app->player->playerCollider->rect;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			app->player->playerInfo.currentDir = LEFT_DIR;
			app->player->UpdateAnimation("walk");
			app->player->isMoving = true;

			if (app->player->playerInfo.speedR == 0)
				app->player->playerInfo.speedR = 1;
			app->player->playerInfo.position.x -= 1 * app->player->playerInfo.speedL;
			app->player->playerCollider->rect.x -= 1 * app->player->playerInfo.speedL;
			app->player->colliderY->rect.x -= 1 * app->player->playerInfo.speedL;
			if (app->render->camera.x < 0 /*&& app->player->playerInfo.position.x <= MAX_LEFT_DISTANCE*/) {
				app->render->camera.x += 6;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			app->player->playerInfo.currentDir = RIGHT_DIR;
			app->player->UpdateAnimation("walk");
			app->player->isMoving = true;

			if (app->player->playerInfo.speedL == 0)
				app->player->playerInfo.speedL = 1;
			app->player->playerInfo.position.x += 1 * app->player->playerInfo.speedR;
			app->player->playerCollider->rect.x += 1 * app->player->playerInfo.speedR;
			app->player->colliderY->rect.x += 1 * app->player->playerInfo.speedR;
			if (app->render->camera.x > -2559 /*&& app->player->playerInfo.position.x >= MAX_RIGHT_DISTANCE*/) {
				app->render->camera.x -= 6;
			}

		}

		//tempPlayerPosition = app->player->playerInfo.position;

		ListItem<MapLayer*>* iteratorLayer = app->map->mapInfo.layers.start;
		while(iteratorLayer != NULL)
		{
			MapLayer* layer = iteratorLayer->data;
			if (strcmp(layer->name.GetString(), "Colision Layer") == 0)
			{
				for (int y = 0; y < app->map->mapInfo.height; y++)
				{
					for (int x = 0; x < app->map->mapInfo.width; x++)
					{
						uint tileId = layer->Get(x, y);
						if (tileId > 0)
						{
							TileSet* set = app->map->GetTilesetFromTileId(tileId);
							SDL_Rect collider = set->GetTileRect(tileId);
							iPoint pos = app->map->MapToWorld(x, y);
							collider.x = pos.x;
							collider.y = pos.y;

							if (app->collision->CheckCollision(collider, app->player->playerCollider->rect))
							{
								switch (tileId)
								{
								case 273://Die
									collision = true;// activate death
									break;

								case 274://Wall
									collision = true;
									GetCollider(collider);
									break;

								case 275://Start
									collision = false;
									//do nothing
									break;

								case 276://End
									collision = true;//change level
									break;

								case 277: //Boost
									//boost player y++
									break;

								case 1://Die
									collision = true;// activate death
									break;

								case 2://Wall
									collision = true;
									break;

								case 3://Start
									collision = false;
									//do nothing
									break;

								case 4://End
									collision = true;//change level
									break;
								}
							}
							else if (app->collision->CheckCollision(collider, app->player->colliderY->rect)) 
							{
								switch (tileId)
								{
								case 273://Die
									collisionY = true;// activate death
									break;

								case 274://Wall
									collisionY = true;
									break;

								case 275://Start
									collisionY = false;
									//do nothing
									break;

								case 276://End
									collisionY = true;//change level
									break;

								case 277: //Boost
									//boost player y++
									break;

								case 1://Die
									collisionY = true;// activate death
									break;

								case 2://Wall
									collisionY = true;
									break;

								case 3://Start
									collisionY = false;
									//do nothing
									break;

								case 4://End
									collisionY = true;//change level
									break;
								}
							}

						}
						//FixPlayerPosition(ColliderFix, app->player->playerCollider->rect);
						if (collision || collisionY) break;
					}
					if (collision || collisionY) break;
				}
				if (collision || collisionY) break;
			}
			if (collision || collisionY) break;
			iteratorLayer = iteratorLayer->next;
		}


		if (collisionY) 
		{
			app->player->onGround = true;
			app->player->playerInfo.position.y = tempPlayerPosition.y;
			app->player->playerCollider->rect.y = app->player->playerInfo.position.y + 5;
			app->player->colliderY->rect.y = app->player->playerInfo.position.y + 19;

			collisionY = false;
		}
		else if (collision && app->player->jumpOn == true) 
		{
			app->player->playerInfo.position.x = tempPlayerPosition.x;
			app->player->playerCollider->rect = tempRectPlayer;
			app->player->colliderY->rect.x = app->player->playerCollider->rect.x;


			collision = false;
		}
		else if (!collisionY)
		{
			app->player->onGround = false;
		}
		else if (!collisionY && collision) 
		{
			app->player->onGround == false;
		}
		else if (collision && !collisionY && app->player->jumpOn)
		{
			collision = false;
		}

		//PLAYER MOVEMENT + COLLISIONS

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (app->player->jumpOn == true && app->player->onGround == true)
			{
				app->player->UpdateAnimation("jump");
				app->player->isMoving = true;
				app->player->jumpHeight = app->player->playerInfo.position.y;
				app->player->Jump(app->player->jumpHeight);
			}
		}

		app->map->Draw();
		app->player->Draw();

		//IDLE ANIMATION
		if (strcmp(app->player->playerInfo.currentAnimation->name.GetString(), "idle") != 0 || strcmp(app->player->playerInfo.currentAnimation->name.GetString(), "idle") != 0)
		{
			if (!app->player->playerInfo.currentAnimation->Finished())
			{
				app->player->playerInfo.currentAnimation->FinishAnimation();
				app->player->UpdateAnimation("idle");
				app->player->isMoving = false;
			}
		}
	}
	else if(titleKey)
	{
		app->render->DrawTexture(introScene, 0, 0);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			introKey = false;
			app->fade->FadeToBlack();
		}
			
	}
	else
	{
		app->render->DrawTexture(titleScene, 0, 0);

		currentTime = SDL_GetTicks();
		if (currentTime > timeTitle)
		{
			titleKey = true;
			app->fade->FadeToBlack();
		}
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

void Scene::FixPlayerPosition(SDL_Rect& r1, SDL_Rect& rp) 
{
	if (rp.y < r1.y + r1.h) 
	{
		app->player->playerInfo.position.y = r1.y + r1.h;
		app->player->playerCollider->rect.y = app->player->playerInfo.position.y + 5;
		app->player->colliderY->rect.y = app->player->playerInfo.position.y + 19;
	}
}

void Scene::GetCollider(SDL_Rect& r) 
{
	ColliderFix = r;
}
