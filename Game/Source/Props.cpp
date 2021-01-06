#include "App.h"
#include "Props.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

Props::Props() : Entity()
{
	enName.Create("props");
	type = PROPS;
}

Props::~Props()
{
}

bool Props::Awake(pugi::xml_node& config)
{
	config = app->scene->sceneConf;
	pugi::xml_node cp = config.child("checkpoints");

	//Set Checkpoints
	checkpoint[0].source.Create(cp.attribute("name").as_string());
	checkpoint[1].source.Create(cp.attribute("name").as_string());

	checkpoint[0].rect.x = cp.child("cp1").attribute("x").as_int();
	checkpoint[0].rect.y = cp.child("cp1").attribute("y").as_int();

	checkpoint[1].rect.x = cp.child("cp2").attribute("x").as_int();
	checkpoint[1].rect.y = cp.child("cp2").attribute("y").as_int();

	cp = config.child("collectible");

	for (int i = 0; i < 4; i++)
	{
		collectible[i].source.Create(cp.attribute("name").as_string());
		collectible[i].itemRect.w = cp.attribute("w").as_int();
		collectible[i].itemRect.h = cp.attribute("h").as_int();
	}
	int i = 0;
	for (pugi::xml_node collect = cp.child("collect"); collect; collect = collect.next_sibling("collect"))
	{
		collectible[i].itemRect.x = collect.attribute("x").as_int();
		collectible[i].itemRect.y = collect.attribute("y").as_int();
		collectible[i].active = collect.attribute("active").as_bool();
		i++;
	}

	cp = config.child("collectiblemark");

	sourceMarker.Create(cp.attribute("name").as_string());

	config = app->enManager->player->playerConf;

	//LIFEGETTER
	for (int i = 0; i < 2; i++)
	{
		lifeGetter[i].source.Create(config.child("lifegetter").attribute("name").as_string());
		lifeGetter[i].getterRect.w = config.child("lifegetter").attribute("w").as_int();
		lifeGetter[i].getterRect.h = config.child("lifegetter").attribute("h").as_int();
	}

	int a = 0;
	for (pugi::xml_node getter = config.child("lifegetter").child("getter"); getter; getter = getter.next_sibling("getter"))
	{
		lifeGetter[a].getterRect.x = getter.attribute("x").as_int();
		lifeGetter[a].getterRect.y = getter.attribute("y").as_int();
		lifeGetter[a].active = getter.attribute("active").as_bool();

		a++;
	}

	return true;
}

bool Props::Start()
{
	for (int i = 0; i < 2; i++)
	{
		checkpoint[i].checked = false;
		checkpoint[i].rect.w = 22;
		checkpoint[i].rect.h = 22;
	}
	checkpoint[0].active = true;

	return true;
}

bool Props::Update(float dt)
{
	return true;
}

bool Props::PostUpdate()
{
	return true;
}

bool Props::CleanUp()
{
	return true;
}

bool Props::Load(pugi::xml_node&)
{
	return true;
}

bool Props::Save(pugi::xml_node&) const
{
	return true;
}

void Props::CollectibleMarkerLogic()
{
	for (int i = 0; i < 4; i++)
	{
		if (collectible[i].collected)
		{
			collectibleCount++;
		}
	}

	if (collectibleCount == 1)
	{
		app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
	}
	else if (collectibleCount == 2)
	{
		app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
	}
	else if (collectibleCount == 3)
	{
		app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 70 - app->render->camera.x / 3, 3);
	}
	else if (collectibleCount == 4)
	{
		app->render->DrawTexture(markerTex, 426 - 30 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 50 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 70 - app->render->camera.x / 3, 3);
		app->render->DrawTexture(markerTex, 426 - 90 - app->render->camera.x / 3, 3);
	}

	collectibleCount = 0;
}
