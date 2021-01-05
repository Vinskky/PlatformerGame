#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "Module.h"
#include "List.h"
#include "Entity.h"

class EnemyManager : public Module
{
public:
	EnemyManager();
	virtual ~EnemyManager();

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

	void DeleteEnemyFly(Entity* enemyFly);
	void DeleteEnemyNormal(Entity* enemyGround);

	void DeleteAllEnemies();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

public:
	List<Entity*> entities;

};

#endif // !_ENEMY_MANAGER_H_

