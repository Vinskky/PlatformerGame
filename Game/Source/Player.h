#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
class Collider;

struct PlayerData
{
	Animation* currentAnimation;
	Animation idle;
	Animation jump;
	Animation die;
	Animation walk;

	iPoint position;
	int speedL;
	int speedR;
	float speedY = 0.001f;
	int jHeight = 1;
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

	void Jump();

	void Draw();

	void Gravity(int &y, int jHeight);

	void OnCollision(Collider* c1, Collider* c2);

public:
	PlayerData playerInfo;
	Collider* playerCollider = nullptr;
	bool onGround;
	
private:	
	SDL_Texture* texture;
	SString textPath;
	iPoint collPlayer = { 7, 3 };
};

#endif
