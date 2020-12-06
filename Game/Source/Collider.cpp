#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Input.h"
#include "Player.h"
#include "Render.h"
#include "Textures.h"
#include "Collider.h"
#include "Scene.h"
#include "Map.h"
#include "EnemyManager.h"
#include "Criature.h"

Collisions::Collisions():Module()
{
	name.Create("collisions");
}

Collisions::~Collisions()
{
}


bool Collisions::Start()
{
	return true;
}

bool Collisions::PreUpdate()
{
	return true;
}

bool Collisions::Update(float dt)
{
	DebugDraw();
	return true;
}

bool Collisions::CleanUp()
{
	ClearColliders();

	return true;
}

bool Collisions::CheckCollision(SDL_Rect r1, SDL_Rect r2) const
{
	return (r1.x < r2.x + r2.w &&
		r1.x + r1.w > r2.x &&
		r1.y < r2.y + r2.h &&
		r1.h + r1.y > r2.y);
}

void Collisions::NoWalkable(SDL_Rect collider)
{
	noWalkable.Add(collider);
}

void Collisions::TriggerDeath(SDL_Rect death)
{
	deathTriggers.Add(death);
}

void Collisions::TriggerWin(SDL_Rect victory)
{
	winTriggers.Add(victory);
}

void Collisions::InitPos(SDL_Rect position)
{
	initPos.Add(position);
}

void Collisions::InitPosEnemy(SDL_Rect position)
{
	initPosEnemyGround.Add(position);
}

void Collisions::InitPosFly(SDL_Rect position)
{
	initPosEnemyFly.Add(position);
}

void Collisions::ClearColliders()
{
	deathTriggers.Clear();
	winTriggers.Clear();
	noWalkable.Clear();
	initPos.Clear();
	initPosEnemyGround.Clear();
	initPosEnemyFly.Clear();
}

void Collisions::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;

	ListItem<SDL_Rect>* noWalkItem = noWalkable.start;
	while (noWalkItem != NULL)
	{
		app->render->DrawRectangle(noWalkItem->data, 0, 255, 0, alpha);
		noWalkItem = noWalkItem->next;
	}
	
	ListItem<SDL_Rect>* deadItem = deathTriggers.start;
	while (deadItem != NULL)
	{
		app->render->DrawRectangle(deadItem->data, 255, 255, 0, alpha);
		deadItem = deadItem->next;
	}

	ListItem<SDL_Rect>* winItem = winTriggers.start;
	while (winItem != NULL)
	{
		app->render->DrawRectangle(winItem->data, 255, 0, 255, alpha);
		winItem = winItem->next;
	}

	ListItem<SDL_Rect>* initItem = initPos.start;
	while (initItem != NULL)
	{
		app->render->DrawRectangle(initItem->data, 0, 0, 255, alpha);
		initItem = initItem->next;
	}

	app->render->DrawRectangle(app->player->playerColider, 155, 155, 255, alpha);

	app->render->DrawRectangle(app->player->swordCollider, 117, 156, 217, alpha);

	if (app->map->lvl1)
	{
		app->render->DrawRectangle(app->scene->checkpoint[0].rect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->scene->collectible[0].itemRect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->scene->collectible[1].itemRect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->player->lifeGetter[0].getterRect, 70, 180, 20, alpha);

		ListItem<Criature*>* enemy = app->enManager->enemies.start;
		for (enemy; enemy != NULL; enemy = enemy->next)
		{
			app->render->DrawRectangle(enemy->data->collider, 180, 20, 180, alpha);
		}

	}
	else if (app->map->lvl2)
	{
		app->render->DrawRectangle(app->scene->checkpoint[1].rect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->scene->collectible[2].itemRect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->scene->collectible[3].itemRect, 180, 20, 20, alpha);
		app->render->DrawRectangle(app->player->lifeGetter[1].getterRect, 70, 180, 20, alpha);
	}







}