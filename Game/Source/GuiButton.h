#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

struct SDL_Texture;

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw();

private:

    // Gui Button specific properties
    // Maybe some animation properties for state change?
    const SDL_Rect Disabled = { 0, 0, 105, 27 };
    const SDL_Rect Normal = { 0, 27, 105, 27 };
    const SDL_Rect Focused = { 0, 54, 105, 27 };
    const SDL_Rect Pressed = { 0, 81, 105, 27 };
};

#endif // __GUIBUTTON_H__
