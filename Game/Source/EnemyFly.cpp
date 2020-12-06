#include "EnemyFly.h"
#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
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
	app->tex->UnLoad(graphics);
	enemyPath.Clear();
	//clear enemy collider
	graphics = nullptr;
}

bool EnemyFly::Awake()
{
	graphics = app->tex->Load("Assets/Textures/bat_sprite_sheet.png");
	enemyVel.Create(0, 0);
	enemyState = ENEMY_IDLE;
	return true;
}

bool EnemyFly::Start()
{
	currAnimation = &idle;
	enemyState = ENEMY_IDLE;
	playerLastPos = app->player->playerInfo.position;
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

	//MoveEnemy();
	//processPos();

	//collision follows

	//ENEMY DEAD
	if (app->collision->CheckCollision(app->player->swordCollider, collider))
	{
		isDead = true;
	}

	return true;
}

void EnemyFly::MoveEnemy()
{
	iPoint goal = app->player->playerInfo.position;
	iPoint init = enemyPos;

	goal = app->map->WorldToMap(goal.x, goal.y);
	init = app->map->WorldToMap(init.x, init.y);
	if (app->player->playerInfo.position.DistanceTo(enemyPos) < 350)
	{
		enemyPath.Clear();
		if (app->pathfinding->CreatePath(enemyPath, init, goal) != -1)
		{
			iPoint temp = *enemyPath.At(enemyPath.Count() - 1);
			int moveX = temp.x - enemyPos.x;
			if (moveX < 0)
			{
				enemyPos.x -= 1;
			}
			else
			{
				enemyPos.x += 1;
			}
			int moveY = temp.y - enemyPos.y;
			if (moveY < 0)
			{
				enemyPos.y -= 1;
			}
			else
			{
				enemyPos.y += 1;
			}
			if (enemyPos.DistanceTo(temp) < 5)
			{
				iPoint popiPoint;
				enemyPath.Pop(popiPoint);
			}
		}
	}
}

void EnemyFly::ProcessPos()
{
}

void EnemyFly::ProcessGravity()
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

	if (enemyPath.Count() > 0 && app->collision->debug)
	{
		Uint8 alpha = 80;
		for (int i = 0; i < enemyPath.Count(); i++)
		{
			iPoint tmp = *enemyPath.At(i);
			tmp = app->map->MapToWorld(tmp.x, tmp.y);
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

bool EnemyFly::Load(pugi::xml_node& loadNode)
{
	enemyPos.x = loadNode.child("enemyFly").child("enemyPosition").attribute("x").as_int();
	enemyPos.y = loadNode.child("enemyFly").child("enemyPosition").attribute("y").as_int();
	collider.x = loadNode.child("enemyFly").child("enemyCollider").attribute("x").as_int();
	collider.y = loadNode.child("enemyFly").child("enemyCollider").attribute("y").as_int();

	return true;
}

bool EnemyFly::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node enFLy = saveNode.append_child(enName.GetString());
	pugi::xml_node position = enFLy.append_child("enemyPosition");
	pugi::xml_node rect = enFLy.append_child("enemyCollider");

	position.append_attribute("x").set_value(enemyPos.x);
	position.append_attribute("y").set_value(enemyPos.y);

	rect.append_attribute("x").set_value(collider.x);
	rect.append_attribute("y").set_value(collider.y);

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
