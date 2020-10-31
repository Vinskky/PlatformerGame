#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Map.h"
#include "Player.h"
#include "window.h"
#include "Collider.h"
#include "Scene.h"


#include "Defs.h"
#include "Log.h"

#include <math.h>

#define MAX_JUMP_HEIGHT 50
#define TILEHEIGHT 16
#define GRAVITY_LIMITER 0.0297f

Player::Player():Module(),texture(nullptr)
{
	name.Create("player");
}

Player::~Player()
{}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player info.");

	bool ret = true;
	textPath = config.child("source").attribute("name").as_string();
	
	pugi::xml_node frame;
	for (pugi::xml_node anim = config.child("animations").first_child(); anim; anim = anim.next_sibling("animation"))
	{
		SString tmp(anim.attribute("name").as_string());
		if (strcmp(tmp.GetString(), "walk") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.walk.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.walk.speed = anim.attribute("speed").as_float();
			playerInfo.walk.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(),"idle") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.idle.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.idle.speed = anim.attribute("speed").as_float();
			playerInfo.idle.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "jump") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.jump.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.jump.speed = anim.attribute("speed").as_float();
			playerInfo.jump.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "die") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.die.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.die.speed = anim.attribute("speed").as_float();
			playerInfo.die.loop = anim.attribute("loop").as_bool();
		}
		else if(strcmp(tmp.GetString(), "walkLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.walkLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.walkLeft.speed = anim.attribute("speed").as_float();
			playerInfo.walkLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "idleLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.idleLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.idleLeft.speed = anim.attribute("speed").as_float();
			playerInfo.idleLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "jumpLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.jumpLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.jumpLeft.speed = anim.attribute("speed").as_float();
			playerInfo.jumpLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "dieLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.dieLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
			}
			playerInfo.dieLeft.speed = anim.attribute("speed").as_float();
			playerInfo.dieLeft.loop = anim.attribute("loop").as_bool();
		}
	}
	
	return ret;
}

bool Player::Start() 
{
	SetInitialPlayer(LVL_1);

	return true;
}

bool Player::Update(float dt) 
{
	//Draw();
	//playerCollider->SetPosition(playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y);

	//LOG("onGround: %d", onGround);
	//Gravity(1);
	
	return true;
}

bool Player::PostUpdate() 
{
	
	return true;
}

bool Player::CleanUp() 
{

	return true;
}

bool Player::Load(pugi::xml_node& load)
{
	playerInfo.position.x = load.child("position").attribute("x").as_int();
	playerInfo.position.y = load.child("position").attribute("y").as_int();
	playerCollider->SetPosition(load.child("collider").attribute("x").as_int(), load.child("collider").attribute("y").as_int());
	colliderY->SetPosition(load.child("collider").attribute("x").as_int(), load.child("collider").attribute("y").as_int());
	playerInfo.currentLevel = (Level)load.child("level").attribute("value").as_int();
	playerInfo.currentDir = (Direction)load.child("direction").attribute("value").as_int();

	LoadCurrentLevel(playerInfo.currentLevel);

	return true;
}

bool Player::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node position = saveNode.append_child("position");
	pugi::xml_node rect = saveNode.append_child("collider");
	pugi::xml_node currentLvl = saveNode.append_child("level");
	pugi::xml_node currentDir = saveNode.append_child("direction");

	position.append_attribute("x").set_value(playerInfo.position.x);
	position.append_attribute("y").set_value(playerInfo.position.y);

	rect.append_attribute("x").set_value(playerCollider->rect.x);
	rect.append_attribute("y").set_value(playerCollider->rect.y);

	rect.append_attribute("x").set_value(colliderY->rect.x);
	rect.append_attribute("y").set_value(colliderY->rect.y);

	currentLvl.append_attribute("value").set_value(playerInfo.currentLevel);
	currentDir.append_attribute("value").set_value(playerInfo.currentDir);

	return true;
}


void Player::SetInitialPlayer(Level lvl)
{
	app->render->camera.x = 0;
	playerInfo.position = { app->map->GetPlayerInitialPos() };
	playerInfo.speedL = 2;
	playerInfo.speedR = 2;
	playerInfo.currentLevel = lvl;
	playerInfo.currentDir = RIGHT_DIR;
	playerCollider = app->collision->AddCollider({ playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y + 5, 10, 17 }, Type::PLAYER, this);
	colliderY = app->collision->AddCollider({ playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y, 10,10 }, Type::PLAYER, this);
	playerInfo.position.y = playerInfo.position.y + (TILEHEIGHT - app->player->colliderY->rect.h - 19);
	playerCollider->rect.y = playerInfo.position.y + 5;
	colliderY->rect.y = playerInfo.position.y + 19;
	texture = app->tex->Load(textPath.GetString());

	playerInfo.currentAnimation = &playerInfo.idle;
}

void Player::Draw()
{
	app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
}

void Player::Gravity()
{
	playerInfo.position.y += gravity;
	//jumpForce = 0;

	playerInfo.position.y -= jumpForce;

	playerCollider->rect.y = playerInfo.position.y + 5;
	colliderY->rect.y = playerInfo.position.y + 19;
}

void Player::Jump(int jumpHeight)
{
	if (playerInfo.position.y <= jumpHeight) {
		playerInfo.position.y -= jumpForce + 40;
		playerCollider->rect.y = playerInfo.position.y + 5;
		colliderY->rect.y = playerInfo.position.y + 19;

		playerInfo.position.y += gravity;

		playerCollider->rect.y = playerInfo.position.y + 5;
		colliderY->rect.y = playerInfo.position.y + 19;
	}
}

void Player::OnCollision(Collider* c1, Collider* c2)
{

}

void Player::ChangeLevel(Level currentLvl)
{
	if (currentLvl == LVL_1)
	{
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->collision->CleanUp();

		if (app->map->Load(app->map->GetLevel2Load().GetString()));
		{
			SetInitialPlayer(LVL_2);
		}
			
	}
	else if (currentLvl == LVL_2)
	{
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->collision->CleanUp();

		if (app->map->Load(app->map->GetLevel2Load().GetString()));
		{
			SetInitialPlayer(LVL_1);
		}
	}
}

void Player::LoadCurrentLevel(Level currentLvl)
{
	if (currentLvl == LVL_1)
	{
		app->map->CleanUp();
		app->map->lvl1 = true;
		app->map->lvl2 = false;
		app->collision->CleanUp();

		if (app->map->Load(app->map->GetLevel2Load().GetString()));
		{
			SetInitialPlayer(LVL_1);
		}
	}
	else if (currentLvl == LVL_2)
	{
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->collision->CleanUp();

		if(app->map->Load(app->map->GetLevel2Load().GetString()));
		{
			SetInitialPlayer(LVL_2);
		}
	}
}

void Player::UpdateAnimation(char* anim)
{
	if ( playerInfo.currentDir == RIGHT_DIR)
	{
		if (strcmp(anim, "idle") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.idle;
		}
		else if (strcmp(anim, "jump") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.jump;
		}
		else if (strcmp(anim, "walk") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.walk;
		}
		else if (strcmp(anim, "die") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.die;
		}
	}
	else if (playerInfo.currentDir == LEFT_DIR)
	{
		if (strcmp(anim, "idleLeft") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.idleLeft;
		}
		else if (strcmp(anim, "jumpLeft") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.jumpLeft;
		}
		else if (strcmp(anim, "walkLeft") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.walkLeft;
		}
		else if (strcmp(anim, "dieLeft") == 0)
		{
			playerInfo.currentAnimation = &playerInfo.dieLeft;
		}
	}
}
