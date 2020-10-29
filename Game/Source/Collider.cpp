#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collider.h"

#include "Defs.h"
#include "Log.h"

#define MAX_COLLIDERS 400

// ModuleCollision Class

ModuleCollision::ModuleCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[WALL][WALL] = false;
	matrix[WALL][PLAYER] = true;
	matrix[WALL][ENEMY] = false;
	matrix[WALL][START] = true;
	matrix[WALL][END] = true;
	matrix[WALL][DIE] = false;
	matrix[WALL][BOOST] = true;

	matrix[PLAYER][WALL] = true;
	matrix[PLAYER][PLAYER] = false;
	matrix[PLAYER][ENEMY] = true;
	matrix[PLAYER][START] = true;
	matrix[PLAYER][END] = true;
	matrix[PLAYER][DIE] = true;
	matrix[PLAYER][BOOST] = true;

	matrix[ENEMY][WALL] = false;
	matrix[ENEMY][PLAYER] = true;
	matrix[ENEMY][ENEMY] = false;
	matrix[ENEMY][START] = false;
	matrix[ENEMY][END] = false;
	matrix[ENEMY][DIE] = false;

	matrix[START][WALL] = false;
	matrix[START][PLAYER] = true;
	matrix[START][ENEMY] = false;
	matrix[START][START] = false;
	matrix[START][END] = false;
	matrix[START][DIE] = false;
	matrix[START][BOOST] = false;

	matrix[END][WALL] = false;
	matrix[END][PLAYER] = true;
	matrix[END][ENEMY] = false;
	matrix[END][START] = false;
	matrix[END][END] = false;
	matrix[END][DIE] = false;
	matrix[END][BOOST] = false;

	matrix[DIE][WALL] = false;
	matrix[DIE][PLAYER] = true;
	matrix[DIE][ENEMY] = false;
	matrix[DIE][START] = false;
	matrix[DIE][END] = false;
	matrix[DIE][DIE] = false;
	matrix[DIE][BOOST] = false;

	matrix[BOOST][WALL] = false;
	matrix[BOOST][PLAYER] = true;
	matrix[BOOST][ENEMY] = false;
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

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->OnCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return true;

}

bool ModuleCollision::Update(float dt)
{

	Draw();

	return true;
}

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

Collider* ModuleCollision::AddCollider(SDL_Rect rect, Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; i++) 
	{
		if (colliders[i] == nullptr) 
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			++numColliders;
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

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener)
{

}

void Collider::SetPosition(int x, int y) {

	rect.x = x;
	rect.y = y;

}

bool Collider::OnCollision(const SDL_Rect& r) const
{
	if (rect.x > r.x + r.w || rect.x + rect.w < r.x || rect.y > r.y + r.h || rect.y + rect.h < r.y)
		return false;

	LOG("COLLIDING");
	return true;
}