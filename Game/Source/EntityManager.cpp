#include "EntityManager.h"
#include "EnemyFly.h"
#include "EnemyGround.h"
#include "Player.h"
#include "Entity.h"
#include "App.h"
#include "Log.h"
#include "Scene.h"
#include "Collider.h"
#include "Defs.h"
#include "Input.h"

EntityManager::EntityManager()
{
	name.Create("entities");
}

EntityManager::~EntityManager()
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

bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;
	CreatePlayer(config);
	
	return ret;
}

bool EntityManager::Start()
{
	/*ListItem<SDL_Rect>* item = app->collision->initPosEnemyGround.start;
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
	}*/

	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		if (item->data->type = Entity::EntityType::PLAYER)
		{
			item->data->Start();
		}
		item = item->next;
	}
	return true;
}

bool EntityManager::PreUpdate()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->PreUpdate();
		item = item->next;
	}
	return true;
}

bool EntityManager::Update(float dt)
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

bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->PostUpdate();
		item = item->next;
	}
	return true;
}

void EntityManager::CreateEnemyFly(iPoint position)
{
	EnemyFly* enFly = new EnemyFly();
	enFly->Awake();
	enFly->pos = position;
	enFly->Start();
	entities.Add(enFly);

	LOG("Enemy Fly Created!");
}

void EntityManager::CreateEnemyNormal(iPoint position)
{
	EnemyGround* enGround = new EnemyGround();
	enGround->Awake();
	enGround->pos = position;
	enGround->Start();
	entities.Add(enGround);

	LOG("Enemy Ground Created!");
}

void EntityManager::CreatePlayer(pugi::xml_node& config)
{
	player = new Player();
	player->Awake(config);
	entities.Add(player);
}

void EntityManager::DeleteEnemyFly(Entity* enemyFly)
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

void EntityManager::DeleteEnemyNormal(Entity* enemyNormal)
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

void EntityManager::DeletePlayer(Entity* player)
{
	int id = entities.Find(player);
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

void EntityManager::DeleteAllEnemies()
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL )
	{
		if (item->data->type != Entity::EntityType::PLAYER)
		{
			item->data->~Entity();
			entities.Del(item);
		}
		item = item->next;
	}
}

void EntityManager::DeleteAll()
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



bool EntityManager::Load(pugi::xml_node& node)
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->Load(node);
		item = item->next;
	}
	return true;
}

bool EntityManager::Save(pugi::xml_node& node) const
{
	ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->Save(node);
		item = item->next;
	}
	return true;
}


