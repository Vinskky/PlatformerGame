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
	}
	
	return ret;
}

bool Player::Start() 
{
	playerInfo.position = { 63,100 }; //32, 110
	playerInfo.speedL = 1;
	playerInfo.speedR = 1;

	playerCollider = app->collision->AddCollider({playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y, 10, 27}, Type::PLAYER, this);

	texture = app->tex->Load(textPath.GetString());

	playerInfo.currentAnimation = &playerInfo.idle;

	return true;
}

bool Player::Update(float dt) 
{
	//Draw();
	playerCollider->SetPosition(playerInfo.position.x + collPlayer.x, playerInfo.position.y + collPlayer.y);

	LOG("onGround: %d", onGround);

	Gravity(playerInfo.position.y, playerInfo.jHeight);
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

void Player::Jump()
{
}

void Player::Draw()
{
	app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
}

void Player::Gravity(int &y, int jHeight)
{
	y = playerInfo.position.y + (jHeight / (2 * deltaTime * deltaTime))*playerInfo.speedY;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == WALL && c1->type == PLAYER) {
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
}