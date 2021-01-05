#include "EnemyGround.h"
#include "App.h"
#include "Log.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"
#include "Pathfinding.h"
#include "EntityManager.h"

EnemyGround::EnemyGround() : Entity()
{
	enName.Create("enemyNormal");
	type = GROUND;
	//LEFT ANIMATION
	moveLeft.PushBack({ 0, 12, 24, 19 }, "moveLeft");
	moveLeft.PushBack({ 24, 12, 24, 19 }, "moveLeft");
	moveLeft.PushBack({ 48, 12, 24, 19 }, "moveLeft");
	moveLeft.loop = true;
	moveLeft.speed = 0.095;

	//RIGHT ANIMATION
	moveRight.PushBack({ 120, 12, 23, 19 }, "moveRight");
	moveRight.PushBack({ 96, 12, 24, 19 }, "moveRight");
	moveRight.PushBack({ 72, 12, 24, 19 }, "moveRight");
	moveRight.loop = true;
	moveRight.speed = 0.095;
}

EnemyGround::~EnemyGround()
{
	currAnimation = nullptr;
	app->tex->UnLoad(graphics);
	enemyPath.Clear();
	collider = {0, 0,0,0 };
	graphics = nullptr;
}

bool EnemyGround::Awake()
{
	vel.Create(0, 0);
	return true;
}

bool EnemyGround::Start()
{
	graphics = app->tex->Load("Assets/Textures/ground_enemy.png");
	entState = WALK_L;
	pastDest = app->enManager->player->playerInfo.position;
	collider = { pos.x,pos.y, 16,16 };
	return true;
}

bool EnemyGround::PreUpdate()
{
	return true;
}

bool EnemyGround::Update(float dt)
{
	MoveEnemy();

	//ENEMY DEATH
	if (app->collision->CheckCollision(app->enManager->player->swordCollider, collider))
	{
		app->enManager->DeleteEnemyNormal(this);
		isDead = true;
	}

	return true;
}

void EnemyGround::MoveEnemy()
{
	iPoint goal = app->enManager->player->playerInfo.position;
	iPoint init = pos;

	goal = app->map->WorldToMap(goal.x, goal.y+16);
	init = app->map->WorldToMap(init.x, init.y);

	if (app->enManager->player->playerInfo.position.DistanceTo(pos) < 125)
	{
		enemyPath.Clear();
		if (app->pathfinding->CreatePath(enemyPath, init, goal) != -1)
		{
			iPoint tmp = *enemyPath.At(enemyPath.Count() - 1);
			int distanceToMove = tmp.x - pos.x;
			if (app->enManager->player->playerInfo.position.x > pos.x)
				distanceToMove = 1;

			if (distanceToMove < 0 )
			{
				if (pos.x > 176)
				{
					pos.x -= 1;
					collider.x = pos.x;
				}
				entState = WALK_L;
			}
			else if (distanceToMove > 0 )
			{
				if (pos.x < 252)
				{
					pos.x += 1;
					collider.x = pos.x;
				}
				
				entState = WALK_R;
			}
		}
	}
}

void EnemyGround::ProcessPos()
{
	pos.y = pos.y + vel.y;
}

void EnemyGround::ProcessGravity(float dt)
{
}

void EnemyGround::Draw()
{
	switch (entState)
	{
	case WALK_L:
	{
		currAnimation = &moveLeft;
		break;
	}
		
	case WALK_R:
	{
		currAnimation = &moveRight;
		break;
	}
	default:
		break;
	}

	if (app->scene->currentScreen == LVL1 || app->scene->currentScreen == LVL1)
	{
		if (!isDead)
			app->render->DrawTexture(graphics, pos.x - 2, pos.y - 2, &currAnimation->GetCurrentFrame());
	}

	if (enemyPath.Count() > 0 && app->collision->debug)
	{
		Uint8 alpha = 70;
		for (int i = 0; i < enemyPath.Count(); i++)
		{
			iPoint tmp = *enemyPath.At(i);
			tmp = app->map->MapToWorld(tmp.x, tmp.y);
			SDL_Rect t = { tmp.x,tmp.y, 16, 16 };
			app->render->DrawRectangle(t, 0, 155, 255, alpha);
		}
	}
}

bool EnemyGround::PostUpdate()
{
	Draw();
	return true;
}

bool EnemyGround::Load(pugi::xml_node& loadNode)
{
	pos.x = loadNode.child("enemyNormal").child("position").attribute("x").as_int();
	pos.y = loadNode.child("enemyNormal").child("position").attribute("y").as_int();
	collider.x = loadNode.child("enemyNormal").child("enemyCollider").attribute("x").as_int();
	collider.y = loadNode.child("enemyNormal").child("enemyCollider").attribute("y").as_int();

	return true;
}

bool EnemyGround::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node enGround = saveNode.append_child(enName.GetString());
	pugi::xml_node position = enGround.append_child("position");
	pugi::xml_node rect = enGround.append_child("enemyCollider");

	position.append_attribute("x").set_value(pos.x);
	position.append_attribute("y").set_value(pos.y);

	rect.append_attribute("x").set_value(collider.x);
	rect.append_attribute("y").set_value(collider.y);

	return true;
}

int EnemyGround::GetDirection() const
{
	return 0;
}

iPoint EnemyGround::GetPosition() const
{
	return pos;
}

bool EnemyGround::CleanUp()
{
	return true;
}
