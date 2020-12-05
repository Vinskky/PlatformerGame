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

#define MAX_JUMP_HEIGHT 42
#define WINDOW_MIDDLE_S 213
#define WINDOW_MIDDLE_E 1065

Player::Player():Module(),texture(nullptr)
{
	name.Create("player");
}

Player::~Player()
{
}

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

	LOG("%s", config.child("life").attribute("source").as_string());
	playerLife.source.Create(config.child("life").attribute("source").as_string());
	playerLife.lifes = config.child("life").attribute("lifes").as_int();
	
	return ret;
}

bool Player::Start() 
{
	SetInitialPlayer(LVL_1);
	return true;
}

bool Player::Update(float dt) 
{
	bool ret = true;

	//LOG("CAMX: %d", app->render->camera.x);

	if (app->scene->currentScreen == Screens::PLAYING)
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
				playerColider.x -= 1 * playerInfo.speed;
				if (CheckCollision() == false)
				{
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x += 6;
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
					playerColider.x -= 1 * playerInfo.speed;
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x += 6;
				}
			}
			UpdateAnimation("walk");
			playerInfo.currentDir = LEFT_DIR;

		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (godMode == false)
			{
				playerColider.x += 1 * playerInfo.speed;
				if (CheckCollision() == false)
				{
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x -= 6;
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
					playerColider.x += 1 * playerInfo.speed;
					playerInfo.position.x = playerColider.x;
					isMoving = true;
					if (playerInfo.position.x >= WINDOW_MIDDLE_S && playerInfo.position.x <= WINDOW_MIDDLE_E)
						app->render->camera.x -= 6;
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

		//ATTACK END

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
			isDead = true;
			doubleJump = false;
			onGround = true;
			jumpOn = false;
			Dead();
		}

		if (CheckWin() == true)
		{
			ChangeLevel(playerInfo.currentLevel);
		}

		app->map->Draw();
		app->player->Draw();

		//IDLE ANIMATION
		if (strcmp(playerInfo.currentAnimation->name.GetString(), "idle") != 0 || strcmp(playerInfo.currentAnimation->name.GetString(), "idleLeft") != 0)
		{
			if (!playerInfo.currentAnimation->Finished())
			{
				playerInfo.currentAnimation->FinishAnimation();
				UpdateAnimation("idle");
				isMoving = false;
			}
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
	}
	return true;
}

bool Player::PostUpdate() 
{
	if (app->scene->currentScreen == PLAYING)
	{
		if (playerLife.lifes == 3)
		{
			app->render->DrawTexture(playerLife.lifeTex, 30 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(playerLife.lifeTex, 50 - app->render->camera.x / 3, 3);
			app->render->DrawTexture(playerLife.lifeTex, 70 - app->render->camera.x / 3, 3);
		}
		else if (playerLife.lifes == 2)
		{

		}
		else if (playerLife.lifes == 1)
		{

		}
	}
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
	playerColider.x = load.child("collider").attribute("x").as_int();
	playerColider.y = load.child("collider").attribute("y").as_int();
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

	rect.append_attribute("x").set_value(playerColider.x);
	rect.append_attribute("y").set_value(playerColider.y);

	currentLvl.append_attribute("value").set_value(playerInfo.currentLevel);
	currentDir.append_attribute("value").set_value(playerInfo.currentDir);

	return true;
}

void Player::SetInitialPlayer(Level lvl)
{
	playerLife.lifeTex = app->tex->Load(playerLife.source.GetString());
	swordCollider = {-20, 0, 13, 7};
	if (!app->scene->checkpoint[0].checked && !app->scene->checkpoint[1].checked)
	{
		app->render->camera.x = 0;
		if (app->IsLoading() == false)
		{
			playerInfo.position = { app->map->GetPlayerInitialPos() };
			playerInfo.position.y += 2;
			playerColider = { playerInfo.position.x + 2, playerInfo.position.y, 14, 30 };

			playerInfo.speed = 2;
			playerInfo.currentLevel = lvl;
			playerInfo.currentDir = RIGHT_DIR;
		}
		texture = app->tex->Load(textPath.GetString());

		playerInfo.currentAnimation = &playerInfo.idle;
	}
	else if (app->scene->checkpoint[0].checked && lvl == LVL_1)
	{
		app->render->camera.x = -2364;
		if (app->IsLoading() == false)
		{
			playerInfo.position = { app->scene->checkpoint[0].rect.x, app->scene->checkpoint[0].rect.y - 16 };
			playerColider = { playerInfo.position.x + 2, playerInfo.position.y, 14, 30 };

			playerInfo.speed = 2;
			playerInfo.currentLevel = lvl;
			playerInfo.currentDir = RIGHT_DIR;
		}
		texture = app->tex->Load(textPath.GetString());

		playerInfo.currentAnimation = &playerInfo.idle;
	}
	else if (app->scene->checkpoint[1].checked && lvl == LVL_2)
	{
		app->render->camera.x = -765;
		if (app->IsLoading() == false)
		{
			playerInfo.position = { app->scene->checkpoint[1].rect.x, app->scene->checkpoint[1].rect.y - 16 };
			playerColider = { playerInfo.position.x + 2, playerInfo.position.y, 14, 30 };

			playerInfo.speed = 2;
			playerInfo.currentLevel = lvl;
			playerInfo.currentDir = RIGHT_DIR;
		}
		texture = app->tex->Load(textPath.GetString());

		playerInfo.currentAnimation = &playerInfo.idle;
	}
}

void Player::Draw()
{
	app->render->DrawTexture(texture, playerInfo.position.x, playerInfo.position.y, &(playerInfo.currentAnimation->GetCurrentFrame()));
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
	app->player->UpdateAnimation("die");
	app->player->isMoving = false;
	app->scene->currentScreen = DEAD_SCREEN;
	LoadCurrentLevel(app->player->playerInfo.currentLevel);
	isDead = true;
}

bool Player::IsDead() const
{
	return isDead;
}

void Player::SetIsDead(bool set)
{
	isDead = set;
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

void Player::ChangeLevel(Level currentLvl)
{
	if (currentLvl == LVL_1)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->scene->checkpoint[0].active = false;
		app->scene->checkpoint[1].active = true;
		//RESTART CHECKPOINTS
		app->scene->checkpoint[0].checked = false;
		app->scene->checkpoint[1].checked = false;

		if (app->map->Load(app->map->GetLevelToLoad().GetString()));
		{
			SetInitialPlayer(LVL_2);
		}
			
	}
	else if (currentLvl == LVL_2)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = true;
		app->map->lvl2 = false;
		app->scene->checkpoint[0].active = true;
		app->scene->checkpoint[1].active = false;
		// RESTART CHECKPOINTS
		app->scene->checkpoint[0].checked = false;
		app->scene->checkpoint[1].checked = false;

		if (app->map->Load(app->map->GetLevelToLoad().GetString()));
		{
			SetInitialPlayer(LVL_1);
		}
	}
}

void Player::LoadCurrentLevel(Level currentLvl)
{
	if (currentLvl == LVL_1)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = true;
		app->map->lvl2 = false;
		app->scene->checkpoint[0].active = true;
		app->scene->checkpoint[1].active = false;
		
		if (app->map->Load(app->map->GetLevelToLoad().GetString()));
		{
			SetInitialPlayer(LVL_1);
		}
	}
	else if (currentLvl == LVL_2)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->scene->checkpoint[0].active = false;
		app->scene->checkpoint[1].active = true;

		if(app->map->Load(app->map->GetLevelToLoad().GetString()));
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

void Player::TP(Cp cp)
{
	if (cp == CP1)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = true;
		app->map->lvl2 = false;
		app->scene->checkpoint[0].active = true;
		app->scene->checkpoint[1].active = false;

		if (app->map->Load(app->map->GetLevelToLoad().GetString()));
		{
			app->render->camera.x = -2364;
			if (app->IsLoading() == false)
			{
				playerInfo.position = { app->scene->checkpoint[0].rect.x, app->scene->checkpoint[0].rect.y - 16 };
				playerColider = { playerInfo.position.x + 2, playerInfo.position.y, 14, 30 };

				playerInfo.speed = 2;
				playerInfo.currentLevel = LVL_1;
				playerInfo.currentDir = RIGHT_DIR;
			}
			texture = app->tex->Load(textPath.GetString());

			playerInfo.currentAnimation = &playerInfo.idle;
		}
	}
	else if (cp == CP2)
	{
		app->collision->CleanUp();
		app->map->CleanUp();
		app->map->lvl1 = false;
		app->map->lvl2 = true;
		app->scene->checkpoint[0].active = false;
		app->scene->checkpoint[1].active = true;

		if (app->map->Load(app->map->GetLevelToLoad().GetString()));
		{
			app->render->camera.x = -765;
			if (app->IsLoading() == false)
			{
				playerInfo.position = { app->scene->checkpoint[1].rect.x, app->scene->checkpoint[1].rect.y - 16 };
				playerColider = { playerInfo.position.x + 2, playerInfo.position.y, 14, 30 };

				playerInfo.speed = 2;
				playerInfo.currentLevel = LVL_2;
				playerInfo.currentDir = RIGHT_DIR;
			}
			texture = app->tex->Load(textPath.GetString());

			playerInfo.currentAnimation = &playerInfo.idle;
		}
	}
}
