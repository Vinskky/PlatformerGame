#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "List.h"

struct SDL_Rect;


class Collisions : public Module
{
public:
	List<SDL_Rect> noWalkable;
	List<SDL_Rect> deathTriggers;
	List<SDL_Rect> winTriggers;
	List<SDL_Rect> initPos;


public:

	Collisions();

	// Destructor
	virtual ~Collisions();


	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);
	// Called before quitting
	bool CleanUp();

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const;
	void NoWalkable(SDL_Rect collider);
	void TriggerDeath(SDL_Rect death);
	void TriggerWin(SDL_Rect victory);
	void InitPos(SDL_Rect position);
	void ClearColliders();

	void DebugDraw();

	// Save & Load
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

private:
	bool debug = false;
};

#endif
