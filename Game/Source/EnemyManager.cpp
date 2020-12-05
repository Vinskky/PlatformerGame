#include "EnemyManager.h"
#include "App.h"
#include "Log.h"
#include "Defs.h"
#include "Input.h"

EnemyManager::EnemyManager()
{
	name.Create("Criatures");
}

EnemyManager::~EnemyManager()
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->~Criature();
		enemies.Del(item);
		item = item->next;
	}
	enemies.Clear();
}

bool EnemyManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool EnemyManager::Start()
{
	return true;
}

bool EnemyManager::PreUpdate()
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->PreUpdate();
		item = item->next;
	}
	return true;
}

bool EnemyManager::Update(float dt)
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		item = item->next;
	}
	return true;
}

bool EnemyManager::PostUpdate()
{
	ListItem<Criature*>* item = enemies.start;
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
	enemies.Add(enFly);

	LOG("Enemy Fly Created!");
}

void EnemyManager::CreateEnemyNormal(iPoint position)
{
}

void EnemyManager::DeleteEnemyFly(Criature* enemyFly)
{
	int id = enemies.Find(enemyFly);
	int ds = 0;
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		if (id == ds)
		{
			item->data->~Criature();
			enemies.Del(item);
			return;
		}
		ds++;
		item = item->next;
	}
}

void EnemyManager::DeleteEnemyNormal(Criature* enemy_normal)
{
}

void EnemyManager::DeleteAllEnemies()
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->~Criature();
		enemies.Del(item);
		item = item->next;
	}
}

bool EnemyManager::Load(pugi::xml_node& node)
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->Load(node);
		item = item->next;
	}
	return true;
}

bool EnemyManager::Save(pugi::xml_node& node) const
{
	ListItem<Criature*>* item = enemies.start;
	while (item != NULL)
	{
		item->data->Save(node);
		item = item->next;
	}
	return true;
}

