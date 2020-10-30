#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	iPoint tempPlayerPosition;
	SString sourceIntro;
	SDL_Texture* introScene;
	SDL_Texture* deathScene;
	SDL_Rect tempRectPlayer;
	bool introKey;
	uint collisionLayer;
	bool collision;
};

#endif // __SCENE_H__