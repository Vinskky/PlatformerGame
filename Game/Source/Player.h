#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
class Collider;

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

	iPoint position;
	int speedL;
	int speedR;
	float speedY = 0.001f;
	int jHeight = 1;
	Direction currentDir;
	Level currentLevel;
	//iPoint posOnWin;
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
	bool Save(pugi::xml_node&)const;

	void Jump(int jupmHeight);

	void SetInitialPlayer(Level lvl);

	void Draw();

	void Gravity();

	void Dead();
	bool IsDead()const;
	void SetIsDead(bool set);

	void OnCollision(Collider* c1, Collider* c2);

	void ChangeLevel(Level currentLvl);
	void LoadCurrentLevel(Level currentLvl);

	void UpdateAnimation(char* anim);

public:
	PlayerData playerInfo;
	Collider* playerCollider = nullptr;
	Collider* colliderY = nullptr;
	bool onGround = true;
	bool jumpOn = true;
	uint jumpHeight = 0;
	bool isMoving;
private:	
	SDL_Texture* texture;
	SString textPath;
	iPoint collPlayer = { 7, 3 };
	uint jumpForce = 8;
	uint jumpForceValue = 8;

	uint gravity = 11;
	uint gravityValue = 11;
	bool isDead = false;
};

#endif
