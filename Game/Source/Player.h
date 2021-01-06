#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "List.h"
#include "Point.h"
#include "Animation.h"
#include "Scene.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

enum Direction
{
	RIGHT_DIR = 1,
	LEFT_DIR
};

enum Level
{
	LVL_1 = 1,
	LVL_2
};

struct PlayerData
{
	Animation* currentAnimation;
	Animation idle;
	Animation jump;
	Animation die;
	Animation walk;
	Animation idleLeft;
	Animation jumpLeft;
	Animation dieLeft;
	Animation walkLeft;
	Animation attackRight;
	Animation attackLeft;

	bool attacking = false;

	iPoint position;
	uint speed;
	Direction currentDir;
	Level currentLevel;
};

struct Life
{
	SDL_Texture* lifeTex;
	int lifes;
	SString source;
};

class Player : public Entity 
{
public:

	//constructor
	Player();

	//Destructor
	virtual ~Player();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	//Save and load Methods
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Jump();

	void Draw();

	void Gravity();

	void Dead();
	bool IsDead() const;

	bool CheckCollision();
	bool CheckDeath();
	bool CheckWin();
	void CheckHurt();

	void UpdateAnimation(char* anim);

	void InvincibleTimer();

public:
	PlayerData playerInfo;
	Life playerLife;
	SDL_Rect playerColider;
	SDL_Rect swordCollider;
	bool onGround = true;
	bool jumpOn = true;
	bool doubleJump = true;
	bool godMode = false;
	bool isMoving;
	bool isInvincible = false;
	uint invincibleTimer;
	pugi::xml_node playerConf;
	SDL_Texture* texture;
	SString textPath;
	bool isDead = false;
};

#endif
