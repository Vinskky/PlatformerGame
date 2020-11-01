#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	SString name;
private:
	float currentFrame;
	int lastFrame = 0;
	int loops = 0;

public:
	void ResetLoops(const int a)
	{
		loops = a;
	}

	void PushBack(const SDL_Rect& rect , SString nameAnim)
	{
		name = nameAnim;
		frames[lastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		currentFrame += speed;
		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			loops++;
		}

		return frames[(int)currentFrame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		loops = 0;
	}

	bool FinishAnimation()
	{
		if (currentFrame >= lastFrame - 0.5f)
		{
			currentFrame = 0;
			return true;
		}
		return false;
	}
};

#endif
