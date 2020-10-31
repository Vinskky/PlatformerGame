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

	void FixPlayerPosition(SDL_Rect& r1, SDL_Rect& r2);

	void GetCollider(SDL_Rect& r);

	SDL_Rect ColliderFix;

private:
	iPoint tempPlayerPosition;
	SString sourceTitle;
	SString sourceDeath;
	SString sourceIntro;
	SDL_Texture* titleScene;
	SDL_Texture* introScene;
	SDL_Texture* deathScene;
	SDL_Rect tempRectPlayer;
	bool introKey;
	bool titleKey;
	uint collisionLayer;
	bool collision;
	bool collisionY;

	Uint32 currentTime;
	Uint32 timeTitle = 3000;
};

#endif // __SCENE_H__