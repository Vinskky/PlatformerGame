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
	currAnimation = &idleLeft;
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
	if (playerLastPos.DistanceTo(app->player->playerInfo.position) > 10)
	{
		if (aStar->CreatePath(enemyPos, app->player->playerInfo.position) != -1)
			playerLastPos = app->player->playerInfo.position;
	}

	MoveEnemy(dt);
	processPos();

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
}

bool EnemyFly::PostUpdate()
{
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
