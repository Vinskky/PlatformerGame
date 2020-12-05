#ifndef _CRIATURE_H_
#define _CRIATURE_H_


#include "Collider.h"
#include "Point.h"

struct SDL_Texture;

enum EnemyState
{
	ENEMY_IDLE,
	ENEMY_WALK_L,
	ENEMY_WALK_R,
	ENEMY_FLY_L,
	ENEMY_FLY_R,
	DEAD
};

enum EnemyDirection
{
	NON,
	ENEMY_UP,
	ENEMY_DOWN,
	ENEMY_LEFT,
	ENEMY_RIGHT
};

class Criature
{
public:
	enum EnemyType
	{
		EN_NORMAL,
		EN_FLY
	};
	Criature() {};
	virtual ~Criature() {};

	//Called before render is available
	virtual bool Awake()
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	//virtual load/save function
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&)const
	{
		return true;
	}

	virtual void OnCollision(Collisions*, Collisions*) {}

public:

	EnemyState enemyState;
	EnemyDirection enemyDirection;

	iPoint enemyPos;
	iPoint enemyVel;

	bool isFlying = false;
	bool isInPlatform = false;

	SString enName;
	bool active = false;

	EnemyType type;
	Collisions* enemyCollider = nullptr;

};

#endif // !_CRIATURE_H_