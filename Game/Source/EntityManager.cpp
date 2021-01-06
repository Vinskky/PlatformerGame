#include "EntityManager.h"
#include "EnemyFly.h"
#include "EnemyGround.h"
#include "Player.h"
#include "Props.h"
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

	CreateProps(config);
	
	return ret;
}

bool EntityManager::Start()
{
	ListItem<Entity*>* item1 = entities.start;
	while (item1 != NULL)
	{
		if (item1->data->type = Entity::EntityType::PLAYER)
		{
			item1->data->Start();
		}
		else if (item1->data->type = Entity::EntityType::PROPS)
		{
			item1->data->Start();
		}
		item1 = item1->next;
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

void EntityManager::CreateProps(pugi::xml_node& config)
{
	props = new Props();
	props->Awake(config);
	entities.Add(props);
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

void EntityManager::DeleteProps(Entity* props)
{
	int id = entities.Find(props);
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
			if (item->data->type != Entity::EntityType::PROPS)
			{
				item->data->~Entity();
				entities.Del(item);
			}
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


