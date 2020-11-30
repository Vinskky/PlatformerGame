#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

enum Screens
{
	TITLE_SCREEN = 1,
	START_SCREEN,
	DEAD_SCREEN,
	PLAYING
};

enum Cp
{
	CP1 = 0,
	CP2,
};

struct Checkpoint
{
	SDL_Rect rect;
	Cp cp;
	bool checked;
	bool active = false;
	void TP(Cp cp);
};

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
	int timer = 0;
	iPoint tempPlayerPosition;
	SString sourceTitle;
	SString sourceDeath;
	SString sourceIntro;
	SDL_Texture* titleScene;
	SDL_Texture* introScene;
	SDL_Texture* deathScene;

public:
	Screens currentScreen;
	Checkpoint checkpoint[2];
};

#endif // __SCENE_H__