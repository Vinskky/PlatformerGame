#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "App.h"
#include "SDL/include/SDL.h"

#define MAX_COLLIDERS 400

class Player;

enum Type
{
	NONE = -1,
	WALL,
	PLAYER,
	START,
	END,
	DIE,
	BOOST,

	MAX
};

struct Collider
{
public:

	Collider(SDL_Rect rectangle, Type type, Player* listener = nullptr) : rect(rectangle), type(type), listener(listener) {}

	void SetPosition(int x, int y);

	SDL_Rect rect;
	Type type;
	Player* listener = nullptr;
	bool to_delete = false;

};

class ModuleCollision : public Module
{
public:
	ModuleCollision();
	~ModuleCollision();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	void Draw();

	void DebugDraw();

	Collider* AddCollider(SDL_Rect rectangle, Type type, Player* listener = nullptr);

	void RemoveCollider(Collider* collider);

	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	bool CheckCollision(SDL_Rect& r1, SDL_Rect& r2) const; //TRY: DO NOT ADD REFERENCE (&)

private:

	bool matrix[MAX_COLLIDERS][MAX_COLLIDERS];
	bool debug = false;
	uint numColliders;

};

#endif
