#ifndef _ENEMY_FLY_H
#define _ENEMY_FLY_H

#include "Entity.h"
#include "Point.h"
#include "Module.h"
#include "DynArray.h"
#include "Animation.h"

class PathFinding;

class EnemyFly : public Entity
{
public:
	EnemyFly();
	virtual ~EnemyFly();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	void MoveEnemy();

	void ProcessPos();

	void ProcessGravity();

	void ReturnToZero();

	void Draw();

	// Called before all Updates
	bool PostUpdate();

	//Save and Load functions
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

	int GetDirection() const;

	iPoint Getposition() const;

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* graphics = nullptr;

	Animation* currAnimation = new Animation();
	Animation idle;
	Animation flyRight;
	Animation flyLeft;
	Animation dieRight;
	Animation dieLeft;

};
#endif // !_ENEMY_FLY_H

