#ifndef _ENEMY_NORMAL_H_
#define _ENEMY_NORMAL_H_

#include "Criature.h"
#include "Point.h"
#include "Module.h"
#include "DynArray.h"
#include "Animation.h"

class PathFinding;

class EnemyNormal : public Criature
{
public:
	EnemyNormal();
	virtual ~EnemyNormal();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	void MoveEnemy();
	void processPos();
	void processGravity(float dt);

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
	Animation moveRight;
	Animation moveLeft;

};

#endif // !_ENEMY_NORMAL_H_
