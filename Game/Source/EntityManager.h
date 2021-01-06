#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "Module.h"
#include "List.h"
#include "Entity.h"
#include "Player.h"
#include "Props.h"

class EntityManager : public Module
{
public:
	EntityManager();
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();
	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	void CreateEnemyFly(iPoint position);
	void CreateEnemyNormal(iPoint position);
	void CreatePlayer(pugi::xml_node&);
	void CreateProps(pugi::xml_node&);

	void DeleteEnemyFly(Entity* enemyFly);
	void DeleteEnemyNormal(Entity* enemyGround);
	void DeletePlayer(Entity* player);
	void DeleteProps(Entity* props);

	void DeleteAllEnemies();
	void DeleteAll();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

public:
	List<Entity*> entities;

	Player* player = nullptr;

	Props* props = nullptr;
};

#endif // !_ENEMY_MANAGER_H_

