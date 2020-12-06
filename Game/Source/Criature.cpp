#include "Criature.h"
#include "EnemyNormal.h"

#include "App.h"
#include "Pathfinding.h"

#include "Log.h"

Criature::Criature()
{
	enemyPath.Create(DEFAULT_PATH_LENGTH);
}

Criature::~Criature()
{
	enemyPath.Clear();
	collider = { 0,0,0,0 };
}
