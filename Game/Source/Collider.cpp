#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collider.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

struct Collider;

// ModuleCollision Class

ModuleCollision::ModuleCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[WALL][WALL] = false;
	matrix[WALL][PLAYER] = true;
	matrix[WALL][START] = true;
	matrix[WALL][END] = true;
	matrix[WALL][DIE] = false;
	matrix[WALL][BOOST] = true;

	matrix[PLAYER][WALL] = true;
	matrix[PLAYER][PLAYER] = false;
	matrix[PLAYER][START] = true;
	matrix[PLAYER][END] = true;
	matrix[PLAYER][DIE] = true;
	matrix[PLAYER][BOOST] = true;

	matrix[START][WALL] = false;
	matrix[START][PLAYER] = true;
	matrix[START][START] = false;
	matrix[START][END] = false;
	matrix[START][DIE] = false;
	matrix[START][BOOST] = false;

	matrix[END][WALL] = false;
	matrix[END][PLAYER] = true;
	matrix[END][START] = false;
	matrix[END][END] = false;
	matrix[END][DIE] = false;
	matrix[END][BOOST] = false;

	matrix[DIE][WALL] = false;
	matrix[DIE][PLAYER] = true;
	matrix[DIE][START] = false;
	matrix[DIE][END] = false;
	matrix[DIE][DIE] = false;
	matrix[DIE][BOOST] = false;

	matrix[BOOST][WALL] = false;
	matrix[BOOST][PLAYER] = true;
	matrix[BOOST][START] = false;
	matrix[BOOST][END] = false;
	matrix[BOOST][DIE] = false;
	matrix[BOOST][BOOST] = false;
	
}

ModuleCollision::~ModuleCollision()
{}

bool ModuleCollision::PreUpdate() 
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

bool ModuleCollision::Update(float dt)
{
	DebugDraw();
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{

		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {

			// avoid checking collisions already checked
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->listener)
				{
					c1->listener->OnCollision(c1, c2);
				}

				if (matrix[c2->type][c1->type] && c2->listener)
				{
					c2->listener->OnCollision(c2, c1);
				}
			}
		}
	}

	Draw();

	return true;
}

//TEST
void ModuleCollision::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case PLAYER: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case WALL: // red
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case DIE: // yellow
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		}
	}
}
//TEST

void ModuleCollision::Draw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i) 
	{
		if (colliders[i] == nullptr)
			continue;

		if (colliders[i]->type == 1)
		{
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
		}
	}
}

bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] != nullptr) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, Type type, Player* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; i++) 
	{
		if (colliders[i] == nullptr) 
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			numColliders++;
			break;
		}
	}

	return ret;
}

void ModuleCollision::RemoveCollider(Collider* collider)
{

	for (uint i = 0; i < MAX_COLLIDERS; i++) 
	{
		if (colliders[i] == collider) 
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--numColliders;
		}
	}
}

// Collider Struct

void Collider::SetPosition(int x, int y) {

	rect.x = x;
	rect.y = y;

}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x <= r.x + r.w && rect.x + rect.w >= r.x && rect.y <= r.y + r.h && rect.h + rect.y >= r.y);
}