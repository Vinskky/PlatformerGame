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
	/*enName.Create("enemyFly");
	type = Type::EN_FLY;*/

}

EnemyFly::~EnemyFly()
{
}

bool EnemyFly::Awake()
{
	return false;
}

bool EnemyFly::Start()
{
	return false;
}

bool EnemyFly::PreUpdate()
{
	return false;
}

bool EnemyFly::Update(float dt)
{
	return false;
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
	return false;
}

bool EnemyFly::Load(pugi::xml_node&)
{
	return false;
}

bool EnemyFly::Save(pugi::xml_node&) const
{
	return false;
}

int EnemyFly::GetDirection() const
{
	return 0;
}

iPoint EnemyFly::Getposition() const
{
	return iPoint();
}

bool EnemyFly::CleanUp()
{
	return false;
}
