#include "EnemyFly.h"
#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Pathfinding.h"
//ManagerCriatures

EnemyFly::EnemyFly() : Criature()
{
	enName.Create("enemyFly");
	type = EN_FLY;

	//animation pushes here

}

EnemyFly::~EnemyFly()
{
	currAnimation = nullptr;
	delete aStar;
	app->tex->UnLoad(graphics);
	//clear enemy collider
	graphics = nullptr;
}

bool EnemyFly::Awake()
{
	graphics = app->tex->Load("Assets/textures/bat_sprite_sheet.png");
	enemyVel.Create(0, 0);
	enemyState = ENEMY_IDLE;
	return true;
}

bool EnemyFly::Start()
{
	currAnimation = &idle;
	enemyState = ENEMY_IDLE;
	aStar = new PathFinding();
	playerLastPos = app->player->playerInfo.position;
	//add collider to enemy
	collider = { enemyPos.x,enemyPos.y, 16,16 };
	return true;
}

bool EnemyFly::PreUpdate()
{
	return true;
}

bool EnemyFly::Update(float dt)
{
	/*if (playerLastPos.DistanceTo(app->player->playerInfo.position) > 10)
	{
		if (aStar->CreatePath(enemyPos, app->player->playerInfo.position) != -1)
			playerLastPos = app->player->playerInfo.position;
	}*/

	//MoveEnemy(dt);
	//processPos();

	//collision follows

	return true;
}

void EnemyFly::MoveEnemy(float dt)
{
}

void EnemyFly::processPos()
{
}

void EnemyFly::processGravity()
{
}

void EnemyFly::ReturnToZero()
{
}

void EnemyFly::Draw()
{
	switch (enemyState)
	{
	case ENEMY_IDLE:
	{
		currAnimation = &idle;
		break;
	}
		
	case ENEMY_FLY_L:
	{
		currAnimation = &flyLeft;
		break;
	}
		
	case ENEMY_FLY_R:
	{
		currAnimation = &flyRight;
		break;
	}
		
	case DEAD:
	{
		currAnimation = &dead;
		break;
	}
		
	default:
		break;
	}

	SDL_Rect r = { 0,0,13,9 };
	if (app->scene->currentScreen == PLAYING)
		app->render->DrawTexture(graphics, enemyPos.x, enemyPos.y, &r);

	if (aStar->GetLastPath() != nullptr && app->collision->debug)
	{
		Uint8 alpha = 80;
		for (int i = 0; i < aStar->GetLastPath()->Count(); i++)
		{
			iPoint tmp = *aStar->GetLastPath()->At(i);
			SDL_Rect t = { tmp.x,tmp.y, 16, 16 };
			app->render->DrawRectangle(t, 255, 255, 255, alpha);
		}
	}
	app->render->DrawRectangle(collider, 125, 151, 255, 75);
}

bool EnemyFly::PostUpdate()
{
	Draw();
	return true;
}

bool EnemyFly::Load(pugi::xml_node&)
{
	return true;
}

bool EnemyFly::Save(pugi::xml_node&) const
{
	return true;
}

int EnemyFly::GetDirection() const
{
	return 0;
}

iPoint EnemyFly::Getposition() const
{
	return enemyPos;
}

bool EnemyFly::CleanUp()
{
	return true;
}
