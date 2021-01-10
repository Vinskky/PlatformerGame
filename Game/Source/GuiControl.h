#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"

#include "Point.h"
#include "SString.h"
#include "Fonts.h"

#include "SDL/include/SDL.h"


enum class GuiControlType
{
    BUTTON,
    TOGGLE,
    CHECKBOX,
    SLIDER,
    SLIDERBAR,
    COMBOBOX,
    DROPDOWNBOX,
    INPUTBOX,
    VALUEBOX,
    SPINNER,
    TEXT
};

enum class GuiControlState
{
    DISABLED,
    NORMAL,
    FOCUSED,
    PRESSED,
    SELECTED
};

class GuiControl
{
public:

    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
        type(type),
        state(GuiControlState::NORMAL),
        bounds(bounds),
        text(text) 
    {
        color.r = 255;
        color.g = 255;
        color.b = 255;
        texture = NULL;
    }

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool Draw() const
    {
        return true;
    }

    void SetTexture(SDL_Texture* tex)
    {
        texture = tex;
        section = { 0, 0, 0, 0 };
    }

    void SetObserver(Module* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

public:

    uint32 id;
    GuiControlType type;
    GuiControlState state;

    SString text;
    SDL_Rect bounds;
    SDL_Color color;

    SDL_Texture* texture;
    SDL_Rect section;

    Fonts font;

    Module* observer;
};

#endif // __GUICONTROL_H__