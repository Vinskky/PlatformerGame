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

#define JUMPSPEED 50.0f
const float gravity = 10.0f;
const float deltaTime = 1.0f / 60.0f;

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
	playerInfo.position = { 63,100 }; //32, 110
	playerInfo.speedL = 1;
	playerInfo.speedR = 1;
	playerInfo.currentDir = RIGHT_DIR;
	playerCollider = app->collision->AddCollider({playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y, 10, 27}, Type::PLAYER, this);

	texture = app->tex->Load(textPath.GetString());

	playerInfo.currentAnimation = &playerInfo.idle;

	return true;
}

bool Player::Update(float dt) 
{
	//Draw();
	//playerCollider->SetPosition(playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y);

	//LOG("onGround: %d", onGround);
	//Gravity(playerInfo.position.y, playerInfo.jHeight);
	
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
	playerInfo.currentLevel = (Level)load.child("level").attribute("value").as_int();
	playerInfo.currentDir = (Direction)load.child("direction").attribute("value").as_int();

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

	currentLvl.append_attribute("value").set_value(playerInfo.currentLevel);
	currentDir.append_attribute("value").set_value(playerInfo.currentDir);

	return true;
}

void Player::Jump()
{
	onGround = false;
	playerInfo.position.y += 10;
	playerCollider->rect.y += 10;
}

void Player::Draw()
{
	app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
}

void Player::Gravity(int jHeight)
{
	playerInfo.position.y += (jHeight / (2 * deltaTime * deltaTime))*playerInfo.speedY;
	playerCollider->rect.y = playerInfo.position.y;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	/*
	if (c2->type == WALL && c1->type == PLAYER) 
	{
		if (c1->rect.x > c2->rect.x + c2->rect.w)
		{
			playerInfo.speedL = 0;
		}
		else if (c1->rect.x + c1->rect.w < c2->rect.x)
		{
			playerInfo.speedR = 0;
		}
		else if (c1->rect.y + c1->rect.h == c2->rect.y)
		{
			playerInfo.position.y = c2->rect.y - c1->rect.h - 2;
			playerInfo.position.y--;
		}
	}
	*/
}

void Player::ChangeLevel(Level currentLvl)
{
	if (currentLvl == LVL_1)
	{
		app->map->CleanUp();
		SString path = app->map->GetLevel2Load(LVL_1);
		path += app->map->GetLevel2Load(LVL_2);
		app->map->Load(path.GetString());
		//TODO MARTI: Reset player acording to level 2
	}
	else if (currentLvl == LVL_2)
	{
		app->map->CleanUp();
		SString path = app->map->GetLevel2Load(LVL_2);
		path += app->map->GetLevel2Load(LVL_1);
		app->map->Load(path.GetString());
		//TODO MARTI: Reset player acording to level 1
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
