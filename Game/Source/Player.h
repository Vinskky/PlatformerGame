#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
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

struct LifeGetter
{
	SDL_Texture* getterTex;
	SString source;
	const int refill = 3;
	SDL_Rect getterRect;
	bool active;
};

class Player : public Module 
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

	//Save and load Methods
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Jump();

	void SetInitialPlayer(Level lvl);

	void Draw();

	void Gravity();

	void Dead();
	bool IsDead() const;
	void SetIsDead(bool set);

	bool CheckCollision();
	bool CheckDeath();
	bool CheckWin();

	void ChangeLevel(Level currentLvl);
	void LoadCurrentLevel(Level currentLvl);

	void UpdateAnimation(char* anim);

	void TP(Cp cp);

public:
	PlayerData playerInfo;
	SDL_Rect playerColider;
	SDL_Rect swordCollider;
	bool onGround = true;
	bool jumpOn = true;
	bool doubleJump = true;
	bool godMode = false;
	bool isMoving;
	Life playerLife;
	LifeGetter lifeGetter[2];
	const int velAugm = 30;
private:
	SDL_Texture* texture;
	SString textPath;
	bool isDead = false;
};

#endif
