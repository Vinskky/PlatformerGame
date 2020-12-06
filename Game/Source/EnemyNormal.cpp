#include "EnemyNormal.h"
#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"
#include "Pathfinding.h"

EnemyNormal::EnemyNormal() : Criature()
{
	enName.Create("enemyNormal");
	type = EnemyType::EN_NORMAL;
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


EnemyNormal::~EnemyNormal()
{
	currAnimation = nullptr;
	app->tex->UnLoad(graphics);
	enemyPath.Clear();
	//clear enemy collider
	collider = {0, 0,0,0 };
	graphics = nullptr;
}

bool EnemyNormal::Awake()
{
	enemyVel.Create(0, 0);
	enemyState = ENEMY_IDLE;
	return true;
}

bool EnemyNormal::Start()
{
	graphics = app->tex->Load("Assets/textures/ground_enemy.png");
	enemyState = ENEMY_IDLE;
	playerLastPos = app->player->playerInfo.position;
	//add collider to enemy
	collider = { enemyPos.x,enemyPos.y, 16,16 };
	return true;
}

bool EnemyNormal::PreUpdate()
{
	return true;
}

bool EnemyNormal::Update(float dt)
{
	iPoint goal = app->player->playerInfo.position;
	iPoint init = enemyPos;

	goal = app->map->WorldToMap(goal.x, goal.y);
	init = app->map->WorldToMap(init.x, init.y);

	if (playerLastPos.DistanceTo(goal) > 50)
	{
		if (app->pathfinding->CreatePath(enemyPath,init,goal) != -1)
			playerLastPos = app->player->playerInfo.position;
	}

	MoveEnemy();
	//processPos();
	//processGravity(dt);

	//collision follows

	//ENEMY DEATH
	if (app->collision->CheckCollision(app->player->swordCollider, collider))
	{
		isDead = true;
	}

	return true;
}

void EnemyNormal::MoveEnemy()
{
	iPoint goal = playerLastPos;
	iPoint init = enemyPos;

	goal = app->map->WorldToMap(goal.x, goal.y+16);
	init = app->map->WorldToMap(init.x, init.y);

	if (app->player->playerInfo.position.DistanceTo(enemyPos) < 75)
	{
		if (app->pathfinding->CreatePath(enemyPath, init, goal) != -1)
		{
			iPoint tmp = *enemyPath.At(enemyPath.Count() - 1);
			int distanceToMove = tmp.x - enemyPos.x;
			if (distanceToMove < 0)
			{
				//ceil Rounds x upward, returning the smallest integral value that is not less than x.
				enemyPos.x -= 1;// ceil(-distanceToMove * 0.17);
				collider.x = enemyPos.x;

			}
			else
			{
				enemyPos.x += 1;//ceil(-distanceToMove * 0.17);
				collider.x = enemyPos.x;
			}
		}
	}
}

void EnemyNormal::processPos()
{
	enemyPos.y = enemyPos.y + enemyVel.y;
}

void EnemyNormal::processGravity(float dt)
{
}

void EnemyNormal::Draw()
{
	enemyState = ENEMY_WALK_L;
	//enemyState = ENEMY_WALK_R;
	switch (enemyState)
	{
	case ENEMY_WALK_L:
	{
		currAnimation = &moveLeft;
		break;
	}
		
	case ENEMY_WALK_R:
	{
		currAnimation = &moveRight;
		break;
	}
	default:
		break;
	}

	if(app->scene->currentScreen == PLAYING && !isDead)
		app->render->DrawTexture(graphics, enemyPos.x-2, enemyPos.y - 2, &currAnimation->GetCurrentFrame());

	if (enemyPath.Count() > 0 && app->collision->debug)
	{
		Uint8 alpha = 80;
		for (int i = 0; i < enemyPath.Count(); i++)
		{
			iPoint tmp = *enemyPath.At(i);
			SDL_Rect t = { tmp.x,tmp.y, 16, 16 };
			app->render->DrawRectangle(t, 255, 255, 255, alpha);
		}
	}
}

bool EnemyNormal::PostUpdate()
{
	Draw();
	return true;
}

bool EnemyNormal::Load(pugi::xml_node&)
{
	return true;
}

bool EnemyNormal::Save(pugi::xml_node&) const
{
	return true;
}

int EnemyNormal::GetDirection() const
{
	return 0;
}

iPoint EnemyNormal::GetPosition() const
{
	return enemyPos;
}

bool EnemyNormal::CleanUp()
{
	return true;
}
