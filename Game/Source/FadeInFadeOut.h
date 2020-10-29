#pragma once
#ifndef __FADEIN_FADEOUT_H__
#define __FADEIN_FADEOUT_H__

#include "Module.h"
#include "Defs.h"
#include "Log.h"
#include "SDL\include\SDL_rect.h"

class FadeInFadeOut : public Module
{
public:
	FadeInFadeOut();

	~FadeInFadeOut();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	// Call modules after each loop iteration
	bool PostUpdate();

	bool FadeToBlack(float time = 1.25f);

	bool CheckFadeIn();
	bool CheckFadeOut();

	bool IsFading();

	enum FadeStep
	{
		NONE,
		FADEIN,
		FADEOUT
	};

	FadeStep currentStep = NONE;

private:

	float normalized = 0;

	bool goFade = false;
	bool finnishFirstPart = false;
	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	SDL_Rect screen;
};

#endif //__FADEIN_FADEOUT_H__