#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

struct PlayerData
{
	Animation* currentAnimation;
	Animation idle;
	Animation jump;
	Animation die;
	Animation walk;

	iPoint position;
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

public:
	PlayerData playerInfo;
	
private:	
	SDL_Texture* texture;
	SString textPath;
};

#endif
