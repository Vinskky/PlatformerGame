#ifndef __PROPS_H__
#define __PROPS_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"

#include "Entity.h"
#include "Scene.h"

enum PropType
{
	CHECKPOINT,
	REFILLER,
	COLLECTIBLE
};

struct Checkpoint
{
	SDL_Rect rect;
	bool checked;
	bool active = false;
	SDL_Texture* checkpointTex;
	SString source;
	int collectibles;
};

struct Collectible
{
	SString source;
	SDL_Rect itemRect;
	bool active;
	bool collected = false;
	SDL_Texture* itemTex;
};

struct LifeGetter
{
	SDL_Texture* getterTex;
	SString source;
	const int refill = 3;
	SDL_Rect getterRect;
	bool active;
};

class Props : public Entity
{
public:
	Props();

	//Destructor
	virtual ~Props();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public: //PROPS

	//CHECKPOINTS
	Checkpoint checkpoint[2];

	//COLLECTIBLES
	Collectible collectible[4];
	int collectibleCount = 0;
	SDL_Texture* markerTex;
	SString sourceMarker;

	void CollectibleMarkerLogic();

	//LIFE REFILLERS
	LifeGetter lifeGetter[2];
};

#endif
