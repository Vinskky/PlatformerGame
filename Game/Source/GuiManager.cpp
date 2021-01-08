#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "App.h"
#include "Textures.h"


GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;
	case GuiControlType::BUTTON: control = new GuiButton(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::TEXT: control = new GuiString(); break;

	default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

bool GuiManager::Start()
{
	buttonSpritesheet = app->tex->Load("Assets/textures/button_spritesheet.png");
	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

