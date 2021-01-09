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
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

#define MAX_JUMP_HEIGHT 42
#define WINDOW_MIDDLE_S 213
#define WINDOW_MIDDLE_E 1065
#define INVINCIBLETIME 100

Player::Player():Entity(),texture(nullptr)
{
	enName.Create("player");
	type = PLAYER;
}

Player::~Player()
{
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player info.");

	bool ret = true;
	textPath = config.child("source").attribute("name").as_string();
	playerConf = config;
	
	pugi::xml_node frame;
	for (pugi::xml_node anim = config.child("animations").first_child(); anim; anim = anim.next_sibling("animation"))
	{
		SString tmp(anim.attribute("name").as_string());
		if (strcmp(tmp.GetString(), "walk") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.walk.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.walk.speed = anim.attribute("speed").as_float();
			playerInfo.walk.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(),"idle") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.idle.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.idle.speed = anim.attribute("speed").as_float();
			playerInfo.idle.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "jump") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.jump.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.jump.speed = anim.attribute("speed").as_float();
			playerInfo.jump.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "die") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.die.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.die.speed = anim.attribute("speed").as_float();
			playerInfo.die.loop = anim.attribute("loop").as_bool();
		}
		else if(strcmp(tmp.GetString(), "walkLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.walkLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.walkLeft.speed = anim.attribute("speed").as_float();
			playerInfo.walkLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "idleLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.idleLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.idleLeft.speed = anim.attribute("speed").as_float();
			playerInfo.idleLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "jumpLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.jumpLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.jumpLeft.speed = anim.attribute("speed").as_float();
			playerInfo.jumpLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "dieLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.dieLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.dieLeft.speed = anim.attribute("speed").as_float();
			playerInfo.dieLeft.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "attackRight") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.attackRight.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.attackRight.speed = anim.attribute("speed").as_float();
			playerInfo.attackRight.loop = anim.attribute("loop").as_bool();
		}
		else if (strcmp(tmp.GetString(), "attackLeft") == 0)
		{
			for (frame = anim.child("frame"); frame; frame = frame.next_sibling("frame"))
			{
				playerInfo.attackLeft.PushBack({ frame.attribute("x").as_int(),frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() }, tmp);
			}
			playerInfo.attackLeft.speed = anim.attribute("speed").as_float();
			playerInfo.attackLeft.loop = anim.attribute("loop").as_bool();
		}
	}

	playerLife.source.Create(config.child("life").attribute("source").as_string());
	playerLife.lifes = config.child("life").attribute("lifes").as_int();
	
	return ret;
}

bool Player::Start() 
{
	return true;
}

bool Player::Update(float dt) 
{
	bool ret = true;

	if (app->scene->currentScreen == Screens::LVL1 || app->scene->currentScreen == Screens::LVL2)
	{
		playerColider.x = playerInfo.position.x;
		playerColider.y = playerInfo.position.y;

		//GodMode
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			godMode = !godMode;

		// Player Controls
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (godMode == false)
			{
				playerColider.x -= 1 * playerInfo.speed /** dt * velAugm*/;
				if (CheckCollision() == false)
				{
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x += 6 /** dt * velAugm*/;
				}
				else if (godMode == false)
				{
					playerColider.x = playerInfo.position.x;
					isMoving = false;
				}
			}
			else
			{
				if (playerInfo.position.x > 0)
				{
					playerColider.x -= 1 * playerInfo.speed /** dt * velAugm*/;
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x += 6 /** dt * velAugm*/;
				}
			}
			UpdateAnimation("walk");
			playerInfo.currentAnimation;
			playerInfo.currentDir = LEFT_DIR;

		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (godMode == false)
			{
				playerColider.x += 1 * playerInfo.speed /** dt * velAugm*/;
				if (CheckCollision() == false)
				{
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x -= 6 /** dt * velAugm*/;
				}
				else
				{
					playerColider.x = playerInfo.position.x;
					isMoving = false;
				}
			}
			else
			{
				if (playerInfo.position.x < 1255)
				{
					playerColider.x += 1 * playerInfo.speed /** dt * velAugm*/;
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x -= 6 /** dt * velAugm*/;
				}
			}
			playerInfo.currentDir = RIGHT_DIR;
			UpdateAnimation("walk");
		}

		//ATTACK CODE
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			playerInfo.attacking = true;
		}

		if (playerInfo.attacking)
		{
			if (playerInfo.currentDir == RIGHT_DIR)
			{
				UpdateAnimation("attackRight");
				swordCollider.x = playerColider.x + 14;
				swordCollider.y = playerColider.y + 15;
			}
			if (playerInfo.currentDir == LEFT_DIR)
			{
				UpdateAnimation("attackLeft");
				swordCollider.x = playerColider.x - 12;
				swordCollider.y = playerColider.y + 15;
			}
			if (playerInfo.currentAnimation->currentFrame > 2.4f)
			{
				playerInfo.attacking = false;
			}

		}
		else
		{
			swordCollider.x = -20;
			swordCollider.y = 0;
		}

		// PHYSICS
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && onGround && !jumpOn)
		{
			Jump();
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !doubleJump && jumpOn)
		{
			//Future implementation of double jump
		}
		else
		{
			Gravity();
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			jumpOn = false;
			doubleJump = false;
		}

		if (CheckDeath() == true && godMode == false)
		{
			app->scene->SetScene(DEAD_SCREEN);
		}

		if (godMode)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				if (playerInfo.position.y > 0)
				{
					playerInfo.position.y -= playerInfo.speed;
					playerColider.y = playerInfo.position.y;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				if (playerInfo.position.y < 210)
				{
					playerInfo.position.y += playerInfo.speed;
					playerColider.y = playerInfo.position.y;
				}
			}
		}

		//HURT && INVINCIBILITY
		CheckHurt();
	}
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
	app->scene->SetScene((Screens)load.child("level").attribute("value").as_int());
	playerInfo.position.x = load.child("position").attribute("x").as_int();
	playerInfo.position.y = load.child("position").attribute("y").as_int();
	playerColider.x = load.child("collider").attribute("x").as_int();
	playerColider.y = load.child("collider").attribute("y").as_int();
	playerInfo.currentDir = (Direction)load.child("direction").attribute("value").as_int();
	app->scene->count = load.child("timer").attribute("currentTime").as_int();
	return true;
}

bool Player::Save(pugi::xml_node& saveNode) const
{
	pugi::xml_node position = saveNode.append_child("position");
	pugi::xml_node rect = saveNode.append_child("collider");
	pugi::xml_node currentLvl = saveNode.append_child("level");
	pugi::xml_node currentDir = saveNode.append_child("direction");
	pugi::xml_node timer = saveNode.append_child("timer");

	position.append_attribute("x").set_value(playerInfo.position.x);
	position.append_attribute("y").set_value(playerInfo.position.y);

	rect.append_attribute("x").set_value(playerColider.x);
	rect.append_attribute("y").set_value(playerColider.y);

	currentLvl.append_attribute("value").set_value(app->scene->currentScreen);
	currentDir.append_attribute("value").set_value(playerInfo.currentDir);

	timer.append_attribute("currentTime").set_value(app->scene->count);

	return true;
}

void Player::Draw()
{
	if (!isInvincible)
	{
		app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
	}
	else
	{
		if (app->GetFrameCount() % 2 == 0)
		{
			app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
		}
	}
}

void Player::Gravity()
{
	if (godMode == false)
	{
		playerColider.y += 1 * playerInfo.speed;
		if (CheckCollision() == false)
		{
			playerInfo.position.y = playerColider.y;
		}
		else
		{
			playerColider.y = playerInfo.position.y;
			onGround = true;
		}
	}
}

void Player::Dead()
{
	app->enManager->player->onGround = true;
	app->enManager->player->jumpOn = false;
	app->enManager->player->UpdateAnimation("die");
	app->enManager->player->isMoving = false;
	isDead = true;
}

bool Player::IsDead() const
{
	return isDead;
}

bool Player::CheckCollision()
{
	bool ret = false;

	ListItem<SDL_Rect>* item = app->collision->noWalkable.start;
	for (item; item != app->collision->noWalkable.end; item = item->next)
	{
		ret = app->collision->CheckCollision(playerColider, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

bool Player::CheckDeath()
{
	bool ret = false;

	ListItem<SDL_Rect>* item = app->collision->deathTriggers.start;
	for (item; item != app->collision->deathTriggers.end; item = item->next)
	{
		ret = app->collision->CheckCollision(playerColider, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

bool Player::CheckWin()
{
	bool ret = false;

	ListItem<SDL_Rect>* item = app->collision->winTriggers.start;
	for (item; item != app->collision->winTriggers.end; item = item->next)
	{
		ret = app->collision->CheckCollision(playerColider, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

void Player::Jump()
{
	if (godMode == false)
	{
		app->audio->PlayFx(1);
		int tempY = playerInfo.position.y;
		UpdateAnimation("jump");
		for (int i = 1; i < MAX_JUMP_HEIGHT; i++)
		{
			playerColider.y -= i;
			if (CheckCollision() == false)
			{
				playerInfo.position.y = playerColider.y;
				onGround = false;
			}
			else
			{
				playerColider.y = playerInfo.position.y;
				break;
			}
			Draw();
			if (playerInfo.position.y <= tempY - MAX_JUMP_HEIGHT)
				break;
		}
	}
}

void Player::UpdateAnimation(char* anim)
{
	if ( playerInfo.currentDir == RIGHT_DIR)
	{
		if (strcmp(anim, "idle") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.idle;
		}
		else if (strcmp(anim, "jump") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.jump;
		}
		else if (strcmp(anim, "walk") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.walk;
		}
		else if (strcmp(anim, "die") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.die;
		}
		else if (strcmp(anim, "attackRight") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.attackRight;
		}
	}
	else if (playerInfo.currentDir == LEFT_DIR)
	{
		if (strcmp(anim, "idle") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.idleLeft;
		}
		else if (strcmp(anim, "jump") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.jumpLeft;
		}
		else if (strcmp(anim, "walk") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.walkLeft;
		}
		else if (strcmp(anim, "die") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.dieLeft;
		}
		else if (strcmp(anim, "attackLeft") == 0)
		{
			playerInfo.currentAnimation->FinishAnimation();
			playerInfo.currentAnimation = &playerInfo.attackLeft;
		}
	}
}

void Player::CheckHurt()
{
	ListItem<Entity*>* enemy = app->enManager->entities.start;
	for (enemy; enemy != NULL; enemy = enemy->next)
	{
		if (app->collision->CheckCollision(playerColider, enemy->data->collider))
		{
			if (!isInvincible)
			{
				playerLife.lifes--;
				isInvincible = true;
			}
		}
		if (isInvincible)
		{
			InvincibleTimer();
		}
	}
}

void Player::InvincibleTimer()
{
	if (invincibleTimer < INVINCIBLETIME)
	{
		LOG("TIME: %d", invincibleTimer);
		invincibleTimer++;
	}
	else
	{
		invincibleTimer = 0;
		isInvincible = false;
	}
}



