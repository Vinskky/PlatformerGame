#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "Module.h"
#include "List.h"
#include "Criature.h"

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

	void DeleteEnemyFly(Criature* enemyFly);
	void DeleteEnemyNormal(Criature* enemy_normal);

	void DeleteAllEnemies();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

private:
	List<Criature*> enemies;

};

#endif // !_ENEMY_MANAGER_H_

