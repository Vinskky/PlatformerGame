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
#include "EnemyManager.h"

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
	enemyVel.Create(0, 0);
	return true;
}

bool EnemyGround::Start()
{
	graphics = app->tex->Load("Assets/Textures/ground_enemy.png");
	entState = WALK_L;
	pastDest = app->player->playerInfo.position;
	collider = { enemyPos.x,enemyPos.y, 16,16 };
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
	if (app->collision->CheckCollision(app->player->swordCollider, collider))
	{
		app->enManager->DeleteEnemyNormal(this);
		isDead = true;
	}

	return true;
}

void EnemyGround::MoveEnemy()
{
	iPoint goal = app->player->playerInfo.position;
	iPoint init = enemyPos;

	goal = app->map->WorldToMap(goal.x, goal.y+16);
	init = app->map->WorldToMap(init.x, init.y);

	if (app->player->playerInfo.position.DistanceTo(enemyPos) < 125)
	{
		enemyPath.Clear();
		if (app->pathfinding->CreatePath(enemyPath, init, goal) != -1)
		{
			iPoint tmp = *enemyPath.At(enemyPath.Count() - 1);
			int distanceToMove = tmp.x - enemyPos.x;
			if (app->player->playerInfo.position.x > enemyPos.x)
				distanceToMove = 1;

			if (distanceToMove < 0 )
			{
				if (enemyPos.x > 176)
				{
					enemyPos.x -= 1;
					collider.x = enemyPos.x;
				}
				entState = WALK_L;
			}
			else if (distanceToMove > 0 )
			{
				if (enemyPos.x < 252)
				{
					enemyPos.x += 1;
					collider.x = enemyPos.x;
				}
				
				entState = WALK_R;
			}
		}
	}
}

void EnemyGround::ProcessPos()
{
	enemyPos.y = enemyPos.y + enemyVel.y;
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
			app->render->DrawTexture(graphics, enemyPos.x - 2, enemyPos.y - 2, &currAnimation->GetCurrentFrame());
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
	enemyPos.x = loadNode.child("enemyNormal").child("enemyPosition").attribute("x").as_int();
	enemyPos.y = loadNode.child("enemyNormal").child("enemyPosition").attribute("y").as_int();
	collider.x = loadNode.child("enemyNormal").child("enemyCollider").attribute("x").as_int();
	collider.y = loadNode.child("enemyNormal").child("enemyCollider").attribute("y").as_int();

	return true;
}

bool EnemyGround::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node enGround = saveNode.append_child(enName.GetString());
	pugi::xml_node position = enGround.append_child("enemyPosition");
	pugi::xml_node rect = enGround.append_child("enemyCollider");

	position.append_attribute("x").set_value(enemyPos.x);
	position.append_attribute("y").set_value(enemyPos.y);

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
	return enemyPos;
}

bool EnemyGround::CleanUp()
{
	return true;
}
