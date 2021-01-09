#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw();

    void SetMinValue(int val);
    void SetMaxValue(int val);

    int GetMinValue()const;
    int GetMaxValue()const;

    void SetSlider(SDL_Rect bounds);


private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider;
    int value;

    int minValue;
    int maxValue;
};

#endif // __GUISLIDER_H__
