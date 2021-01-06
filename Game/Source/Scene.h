#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiControl.h"

struct SDL_Texture;
class GuiButton;

enum Screens
{
	TITLE_SCREEN = 1,
	MAIN_MENU,
	DEAD_SCREEN,
	LVL1,
	LVL2,
	CONFIG_MENU,
	PAUSE_MENU,
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	int timer = 0;
	iPoint tempPlayerPosition;

public: //GUI
	GuiButton* playButton;
	GuiButton* configButton;

public:
	//SCENE MANAGER
	Screens currentScreen;
	Screens previousScreen;

	void SceneCleanUp();

	void SetScene(Screens scene);
	void SetTitleScreen();
	void SetMainMenu();
	void SetDeadScreen();
	void SetLvl1();
	void SetLvl2();
	void SetConfigMenu();
	void SetPauseMenu();

	void UpdateTitle();
	void UpdateMainMenu();
	void UpdateDeadScreen();
	void UpdateLevels();
	void UpdateConfigMenu();
	void UpdatePauseMenu();

	bool OnGuiMouseClickEvent(GuiControl* control);

	SString sourceTitle;
	SString sourceDeath;
	SString sourceIntro;

	SDL_Texture* titleScene;
	SDL_Texture* menuScene;
	SDL_Texture* deathScene;
	SDL_Texture* configMenu;
	SDL_Texture* pauseMenu;

	pugi::xml_node sceneConf;
};

#endif // __SCENE_H__