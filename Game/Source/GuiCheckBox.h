#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw();

    bool GetCheckedState() const
    {
        return checked;
    }

private:

    // GuiCheckBox specific properties
    // Maybe some animation properties for state change?
    bool checked = false;
    const SDL_Rect Normal = { 0, 0, 25, 25 };
    const SDL_Rect NormalC = { 0, 25, 25, 25 };
    const SDL_Rect Focused = { 0, 50, 25, 25 };
    const SDL_Rect FocusedC = { 0, 76, 25, 25 };
};

#endif // __GUICHECKBOX_H__
