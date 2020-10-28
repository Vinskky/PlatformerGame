#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Module.h"
#include "App.h"
#include "SDL/include/SDL.h"

#define MAX_COLLIDERS 200

enum Type
{
	NONE = -1,
	WALL,
	PLAYER,
	ENEMY,
	START,
	END,
	DIE,
	BOOST,

	MAX
};

struct Collider
{
public:

	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPosition(int x, int y);

	bool OnCollision(const SDL_Rect& r) const;

	SDL_Rect rect;
	Type type;
	Module* listener = nullptr;
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

	Collider* AddCollider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void RemoveCollider(Collider* collider);

	Collider* colliders[MAX_COLLIDERS] = { nullptr };

private:

	bool matrix[MAX_COLLIDERS][MAX_COLLIDERS];
	bool debug = false;
	uint numColliders;

};

#endif
