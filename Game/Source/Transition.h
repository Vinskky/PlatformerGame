#ifndef __MODULETRANSITION_H__
#define __MODULETRANSITION_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

class Transition : public Module
{
public:

    Transition();

    //Destructor
    ~Transition();

    // Called before the first frame
    bool Start() override;

    // Called each loop iteration
    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();

    // Called from another module
    // Starts the fade process which has two steps, fade_out and fade_in
    // After the first step, the modules should be switched
    bool FadeEffect(bool fadeInOnly, float frames = 60);

private:

    enum Transition_Step
    {
        NONE,
        TO_BLACK,
        FROM_BLACK
    }
    currentStep = Transition_Step::NONE;

    // A frame count system to handle the fade time and ratio
    Uint32 frameCount;
    Uint32 maxFadeFrames;

    // The rectangle of the screen, used to render the black rectangle
    SDL_Rect screenRect;
public:
    bool isFading = false;
};

#endif //__MODULEFADETOBLACK_H__