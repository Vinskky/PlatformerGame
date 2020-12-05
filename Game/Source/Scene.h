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
	SDL_Texture* checkpointTex;
	SString source;
	int collectibles;
};

struct Collectible
{
	SString source;
	SDL_Rect itemRect;
	bool active;
	bool collected = false;
	SDL_Texture* itemTex;
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
	void CollectibleMarkerLogic();
	Screens currentScreen;
	Checkpoint checkpoint[2];
	Collectible collectible[4];

	SDL_Texture* markerTex;
	SString sourceMarker;
	int collectibleCount = 0;
};

#endif // __SCENE_H__