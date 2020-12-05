#include "EnemyNormal.h"
#include "Textures.h"
#include "App.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Render.h"

EnemyNormal::EnemyNormal() : Criature()
{
	enName.Create("enemyNormal");
	type = EnemyType::EN_NORMAL;
	//animations pushes here
	//----------
	//----------
	//----------
	//----------
}


EnemyNormal::~EnemyNormal()
{
	currAnimation = nullptr;
	delete aStar;
	app->tex->UnLoad(graphics);
	//clear enemy collider
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
	//graphics == texture
	//current animation on start
	enemyState = ENEMY_IDLE;
	aStar = new PathFinding();
	playerLastPos = app->player->playerInfo.position;
	if (aStar->CreatePath(enemyPos, playerLastPos) != -1)
		path = aStar->GetLastPath();
	//add collider to enemy

	return true;
}

bool EnemyNormal::PreUpdate()
{
	return true;
}

bool EnemyNormal::Update(float dt)
{
	if (playerLastPos.DistanceTo(app->player->playerInfo.position) > 10)
	{
		if (aStar->CreatePath(enemyPos, app->player->playerInfo.position) != -1)
		{
			path = aStar->GetLastPath();
			playerLastPos = app->player->playerInfo.position;
		}
	}

	MoveEnemy(dt);
	processPos();
	processGravity(dt);

	//collision follows

	return true;
}

void EnemyNormal::MoveEnemy(float dt)
{
	if (app->player->playerInfo.position.DistanceTo(enemyPos) < 250)
	{
		if (path != nullptr)
		{
			iPoint tmp = *path->At(path->Count() - 1);
			int distanceToMove = tmp.x - enemyPos.x;
			if (distanceToMove < 0)
			{
				//ceil Rounds x upward, returning the smallest integral value that is not less than x.
				enemyPos.x -= ceil(-distanceToMove * dt); 
			}
			else
			{
				enemyPos.x += ceil(-distanceToMove * dt);
			}

			if (enemyPos.DistanceTo(tmp) < 5)
			{
				iPoint popiPoint;
				path->Pop(popiPoint);
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

	SDL_Rect r = currAnimation->GetCurrentFrame();
	app->render->DrawTexture(graphics, enemyPos.x, enemyPos.y, &r, 2);

	if (path != nullptr && app->collision->debug)
	{
		Uint8 alpha = 80;
		for (int i = 0; i < path->Count(); i++)
		{
			iPoint tmp = *path->At(i);
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

iPoint EnemyNormal::Getposition() const
{
	return enemyPos;
}

bool EnemyNormal::CleanUp()
{
	return true;
}
