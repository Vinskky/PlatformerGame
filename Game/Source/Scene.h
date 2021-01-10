#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiControl.h"
#include "Fonts.h"

#define LEVEL_TIME 3000 

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

public:
	void UpdateTimer();
	void ResetTimer();
	int count = LEVEL_TIME;
	int time = count / 60;

private:
	int timer = 0;
	iPoint tempPlayerPosition;
	iPoint pausePlayerPosition;
	iPoint pauseEnemyPosition;
	int pauseCamPositionX;
	char timerText[5];

public: //GUI
	bool OnGuiMouseClickEvent(GuiControl* control);

	//MAIN MENU
	GuiButton* playButton = nullptr;
	GuiString* strPlay = nullptr;
	GuiButton* continueButton = nullptr;
	GuiString* strContinue = nullptr;
	GuiButton* configButton = nullptr;
	GuiString* strConfig = nullptr;
	GuiButton* creditsButton = nullptr;
	GuiString* strCredits = nullptr;
	GuiButton* exitMainButton = nullptr;
	GuiString* strExit = nullptr;
	GuiString* strTimer = nullptr;

	//PAUSE MENU
	GuiButton* resumeButton = nullptr;
	GuiString* strResume = nullptr;
	GuiButton* configPauseButton = nullptr;
	GuiString* strConfigPause = nullptr;
	GuiButton* exitPauseButton = nullptr;
	GuiString* strExitPause = nullptr;
	GuiButton* mainMenuPauseButton = nullptr;
	GuiString* strMainMenu = nullptr;

	//CONFIG
	GuiSlider* sliderVolume = nullptr;
	GuiString* strVolume = nullptr;
	GuiString* strVolumeValue = nullptr;

	GuiSlider* sliderFX = nullptr;
	GuiString* strFX = nullptr;
	GuiString* strFXValue = nullptr;

	GuiButton* backToPauseButton = nullptr;
	GuiString* strBackToPause = nullptr;
	GuiCheckBox* vSyncCheckBox = nullptr;
	GuiString* strVsync = nullptr;
	GuiCheckBox* fullScreenCheckBox = nullptr;
	GuiString* strFullScreen = nullptr;

	//CREDITS
	GuiButton* backCreditsButton = nullptr;
	GuiString* strBackCredits = nullptr;

public:

	bool activeContinue;

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

	SDL_Texture* titleScene = nullptr;
	SDL_Texture* menuScene = nullptr;
	SDL_Texture* deathScene = nullptr;
	SDL_Texture* configMenu = nullptr;
	SDL_Texture* pauseMenu = nullptr;
	SDL_Texture* creditsMenu = nullptr;

	pugi::xml_node sceneConf;
};

#endif // __SCENE_H__