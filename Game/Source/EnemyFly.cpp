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

	//LEFT ANIMATION
	flyLeft.PushBack({ 53, 38, 13, 19 }, "moveLeft");
	flyLeft.PushBack({ 38, 38, 15, 19 }, "moveLeft");
	flyLeft.PushBack({ 28, 38, 11, 19 }, "moveLeft");
	flyLeft.PushBack({ 16, 38, 12, 19 }, "moveLeft");
	flyLeft.PushBack({ 0, 38, 16, 19 }, "moveLeft");
	flyLeft.loop = true;
	flyLeft.speed = 0.21;

	//RIGHT ANIMATION
	flyRight.PushBack({ 0, 0, 16, 19 }, "moveRight");
	flyRight.PushBack({ 16, 0, 10, 19 }, "moveRight");
	flyRight.PushBack({ 28, 0, 8, 19 }, "moveRight");
	flyRight.PushBack({ 39, 0, 9, 19 }, "moveRight");
	flyRight.PushBack({ 51, 0, 15, 19 }, "moveRight");
	flyRight.loop = true;
	flyRight.speed = 0.21;

	//RIGHT DEATH ANIMATION
	dieRight.PushBack({ 0, 19, 16, 19 }, "dieRight");
	dieRight.PushBack({ 16, 19, 12, 19 }, "dieRight");
	dieRight.PushBack({ 28, 19, 11, 19 }, "dieRight");
	dieRight.PushBack({ 39, 19, 11, 19 }, "dieRight");
	dieRight.PushBack({ 50, 19, 16, 19 }, "dieRight");
	dieRight.loop = false;
	dieRight.speed = 0.21;

	//LEFT DEATH ANIMATION
	dieLeft.PushBack({ 50, 57, 16, 19 }, "dieLeft");
	dieLeft.PushBack({ 40, 57, 10, 19 }, "dieLeft");
	dieLeft.PushBack({ 30, 57, 10, 19 }, "dieLeft");
	dieLeft.PushBack({ 18, 57, 12, 19 }, "dieLeft");
	dieLeft.PushBack({ 0, 57, 18, 19 }, "dieLeft");
	dieLeft.loop = false;
	dieLeft.speed = 0.21;

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

	//ENEMY DEAD
	if (app->collision->CheckCollision(app->player->swordCollider, collider))
	{
		isDead = true;
	}

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
	enemyState = ENEMY_FLY_L;
	//enemyState = ENEMY_FLY_R;
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
		if (enemyDirection == ENEMY_RIGHT) currAnimation = &dieRight;
		else if (enemyDirection == ENEMY_LEFT) currAnimation = &dieLeft;
		break;
	}
		
	default:
		break;
	}

	if (app->scene->currentScreen == PLAYING && !isDead)
		app->render->DrawTexture(graphics, enemyPos.x, enemyPos.y, &currAnimation->GetCurrentFrame());

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
