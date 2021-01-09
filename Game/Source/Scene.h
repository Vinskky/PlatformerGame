#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiControl.h"
#include "Fonts.h"

struct SDL_Texture;
class GuiButton;
class GuiCheckBox;
class GuiString;
class GuiSlider;
class Fonts;

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
	iPoint pausePlayerPosition;
	iPoint pauseEnemyPosition;
	int pauseCamPositionX;

public: //GUI
	bool OnGuiMouseClickEvent(GuiControl* control);

	//MAIN MENU
	GuiButton* playButton;
	GuiString* strPlay;
	GuiButton* continueButton;
	GuiString* strContinue;
	GuiButton* configButton;
	GuiString* strConfig;
	GuiButton* creditsButton;
	GuiString* strCredits;
	GuiButton* exitMainButton;
	GuiString* strExit;

	//PAUSE MENU
	GuiButton* resumeButton;
	GuiString* strResume;
	GuiButton* configPauseButton;
	GuiString* strConfigPause;
	GuiButton* exitPauseButton;
	GuiString* strExitPause;
	GuiButton* mainMenuPauseButton;
	GuiString* strMainMenu;

	//CONFIG
	GuiSlider* sliderVolume;
	GuiString* strVolume;
	GuiString* strVolumeValue;

	GuiSlider* sliderFX;
	GuiString* strFX;
	GuiString* strFXValue;

	GuiButton* backToPauseButton;
	GuiString* strBackToPause;
	GuiCheckBox* vSyncCheckBox;
	GuiString* strVsync;
	GuiCheckBox* fullScreenCheckBox;
	GuiString* strFullScreen;

public: //TEXT

	Fonts* playMainText;

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

	bool pause = false;
	bool config = false;
	bool credits = false;
	bool exit = false;

	SString sourceTitle;
	SString sourceDeath;
	SString sourceIntro;

	SDL_Texture* titleScene;
	SDL_Texture* menuScene;
	SDL_Texture* deathScene;
	SDL_Texture* configMenu;
	SDL_Texture* pauseMenu;
	SDL_Texture* creditsMenu;

	pugi::xml_node sceneConf;
};

#endif // __SCENE_H__