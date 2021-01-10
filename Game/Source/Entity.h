#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "DynArray.h"
#include "Collider.h"
#include "Point.h"
#include "Render.h"

enum State
{
	IDLE,
	WALK_L,
	WALK_R,
	FLY_L,
	FLY_R,
	JUMP_L,
	JUMP_R,
	DEAD
};

enum EntDirection
{
	NON,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Entity
{
public:
	enum EntityType
	{
		GROUND,
		FLY,
		PLAYER,
		PROPS
	};

	Entity()
	{
	};

	virtual ~Entity()
	{
	};

	//Called before render is available
	virtual bool Awake(pugi::xml_node&)
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


public:

	State entState;
	EntDirection entDirection;

	SDL_Rect collider;
	iPoint pos;
	iPoint vel;

	bool isFlying = false;
	bool isInPlatform = false;
	bool isMove = false;
	bool isDead = false;

	SString enName;
	bool active = false;

	EntityType type;
	DynArray<iPoint> enemyPath;

protected:
	iPoint pastDest;
	int i;
};

#endif // !_CRIATURE_H_