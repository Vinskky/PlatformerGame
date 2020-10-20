#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

struct SDL_Texture;

class Player : public Module {

public:

	//constructor
	Player();

	//Destructor
	virtual ~Player();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:

	int position;

	SDL_Texture* texture;
};

#endif
