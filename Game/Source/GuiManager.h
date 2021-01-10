#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

struct SDL_Texture;

class GuiManager : public Module
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type);

public:

	List<GuiControl*> controls;

	SDL_Texture* buttonSpriteSheet;
	SDL_Texture* checkBoxSpriteSheet;
	SDL_Texture* sliderSpriteSheet;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
	bool debugGui = false;
};

#endif // __GUIMANAGER_H__
