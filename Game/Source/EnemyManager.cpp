#include "EnemyManager.h"
#include "EnemyFly.h"
#include "EnemyGround.h"
#include "Entity.h"
#include "App.h"
#include "Log.h"
#include "Scene.h"
#include "Collider.h"
#include "Defs.h"
#include "Input.h"

EnemyManager::EnemyManager()
{
	name.Create("criatures");
}

EnemyManager::~EnemyManager()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->~Entity();
		entities.Del(item);
		item = item->next;
	}
	entities.Clear();
}

bool EnemyManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool EnemyManager::Start()
{
	ListItem<SDL_Rect>* item = app->collision->initPosEnemyGround.start;
	while (item != NULL)
	{
		iPoint pos = { item->data.x, item->data.y };
		CreateEnemyNormal(pos);
		item = item->next;
	}

	ListItem<SDL_Rect>* item2 = app->collision->initPosEnemyFly.start;
	while (item2 != NULL)
	{
		iPoint pos = { item2->data.x, item2->data.y };
		CreateEnemyFly(pos);
		item2 = item2->next;
	}

	return true;
}

bool EnemyManager::PreUpdate()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->PreUpdate();
		item = item->next;
	}
	return true;
}

bool EnemyManager::Update(float dt)
{
	if(app->scene->currentScreen == LVL1 || app->scene->currentScreen == LVL2)
	{
		ListItem<Entity*>* item = entities.start;
		while (item != NULL)
		{
 			item->data->Update(dt);
			item = item->next;
		}
	}
	return true;	
}

bool EnemyManager::PostUpdate()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->PostUpdate();
		item = item->next;
	}
	return true;
}

void EnemyManager::CreateEnemyFly(iPoint position)
{
	EnemyFly* enFly = new EnemyFly();
	enFly->Awake();
	enFly->enemyPos = position;
	enFly->Start();
	entities.Add(enFly);

	LOG("Enemy Fly Created!");
}

void EnemyManager::CreateEnemyNormal(iPoint position)
{
	EnemyGround* enGround = new EnemyGround();
	enGround->Awake();
	enGround->enemyPos = position;
	enGround->Start();
	entities.Add(enGround);

	LOG("Enemy Ground Created!");
}

void EnemyManager::DeleteEnemyFly(Entity* enemyFly)
{
	int id = entities.Find(enemyFly);
	int ds = 0;
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		if (id == ds)
		{
			item->data->~Entity();
			entities.Del(item);
			return;
		}
		ds++;
		item = item->next;
	}
}

void EnemyManager::DeleteEnemyNormal(Entity* enemyNormal)
{
	int id = entities.Find(enemyNormal);
	int ds = 0;
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		if (id == ds)
		{
			item->data->~Entity();
			entities.Del(item);
			return;
		}
		ds++;
		item = item->next;
	}
}

void EnemyManager::DeleteAllEnemies()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->~Entity();
		entities.Del(item);
		item = item->next;
	}
	entities.Clear();
}

bool EnemyManager::Load(pugi::xml_node& node)
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->Load(node);
		item = item->next;
	}
	return true;
}

bool EnemyManager::Save(pugi::xml_node& node) const
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->Save(node);
		item = item->next;
	}
	return true;
}


