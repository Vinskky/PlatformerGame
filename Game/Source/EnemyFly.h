#ifndef _ENEMY_FLY_H
#define _ENEMY_FLY_H

#include "Criature.h"
#include "Point.h"
#include "Module.h"
#include "DynArray.h"
#include "Animation.h"

class PathFinding;

class EnemyFly : public Criature
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

	void MoveEnemy(float dt);

	void processPos();

	void processGravity();

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
	EnemyState state;
	EnemyDirection direction;
	SDL_Texture* graphics = nullptr;
	SDL_Rect collider;
	bool isMove = false;

	PathFinding* aStar = nullptr;

	iPoint playerLastPos;

	Animation* currAnimation = new Animation();
	Animation idleRight;
	Animation idleLeft;
	Animation flyRight;
	Animation flyLeft;
	Animation deadRight;
	Animation deadLeft;

};
#endif // !_ENEMY_FLY_H

