#include <math.h>
#include "App.h"
#include "FadeInFadeOut.h"
#include "Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Window.h"

FadeInFadeOut::FadeInFadeOut():	Module()
{
	name = "fadeInFadeOut";
}

FadeInFadeOut::~FadeInFadeOut()
{}

bool FadeInFadeOut::Awake(pugi::xml_node& conf)
{

	screen = { 0, 0, conf.attribute("width").as_int(), conf.attribute("height").as_int() };

	return true;
}


bool FadeInFadeOut::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);


	return true;
}

// Update: draw background
bool FadeInFadeOut::Update(float dt)
{
	if (goFade == false)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (currentStep)
	{
		case FadeStep::FADEIN:
		{
			if (now >= totalTime)
			{
				totalTime += totalTime;
				startTime = SDL_GetTicks();
				currentStep = FadeStep::FADEOUT;
				finnishFirstPart = true;
				return true;
			}
		} break;

		case FadeStep::FADEOUT:
		{
			normalized = 1.0f - normalized;

			if (now >= totalTime)
			{
				currentStep = FadeStep::NONE;
				goFade = false;
				finnishFirstPart = false;
				return true;
			}
		} break;
	}
}

bool FadeInFadeOut::IsFading()
{
	return currentStep == NONE;
}

bool FadeInFadeOut::PostUpdate()
{
	// Finally render the black square with alpha on the screen

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);
	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool FadeInFadeOut::FadeToBlack(float time)
{
	bool ret = false;

	if (goFade == false)
	{
		currentStep = FadeStep::FADEIN;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		goFade = true;
		ret = true;
	}

	return ret;
}

bool FadeInFadeOut::CheckFadeIn()
{
	return finnishFirstPart;
}

bool FadeInFadeOut::CheckFadeOut()
{
	if (goFade)
	{
		return false;
	}
	else
	{
		finnishFirstPart = false;
		return true;
	}

}