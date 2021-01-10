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
    void SetValue(int val);
    void UpdateValue();

    int GetMinValue() const;
    int GetMaxValue() const;
    int GetValue() const;
    float GetPercentValue() const;
    void SetSlider(SDL_Rect bounds);


private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider;
    int value;
    float percentValue;

    int minValue;
    int maxValue;

    const SDL_Rect normalBar = { 0, 0, 119, 15 };
    const SDL_Rect pressedBar = { 0, 15, 119, 15 };

    const SDL_Rect normalSlider = { 0, 30, 17, 17 };
    const SDL_Rect pressedSlider = { 18, 30, 17, 17 };
};

#endif // __GUISLIDER_H__
