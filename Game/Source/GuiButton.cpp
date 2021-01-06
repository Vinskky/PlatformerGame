#include "GuiButton.h"
#include "GuiManager.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw()
{
    switch (state)
    {
    case GuiControlState::DISABLED: app->render->DrawTexture(app->guiManager->buttonSpritesheet, bounds.x, bounds.y, &Disabled);
        break;
    case GuiControlState::NORMAL: app->render->DrawTexture(app->guiManager->buttonSpritesheet, bounds.x, bounds.y, &Normal);
        break;
    case GuiControlState::FOCUSED: app->render->DrawTexture(app->guiManager->buttonSpritesheet, bounds.x, bounds.y, &Focused);
        break;
    case GuiControlState::PRESSED: app->render->DrawTexture(app->guiManager->buttonSpritesheet, bounds.x, bounds.y, &Pressed);
        break;
    case GuiControlState::SELECTED: app->render->DrawTexture(app->guiManager->buttonSpritesheet, bounds.x, bounds.y);
        break;
    default:
        break;
    }

    return false;
}
