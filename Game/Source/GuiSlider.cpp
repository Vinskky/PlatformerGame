#include "App.h"
#include "GuiSlider.h"
#include "GuiManager.h"
#include "Audio.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    //Set slider on the middle of the bounds bar.
    slider = { (bounds.x + (int)(bounds.w * 0.5)) - (int)(bounds.h * 0.5), bounds.y, bounds.h,bounds.h };
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > slider.x) && (mouseX < (slider.x + slider.w)) &&
            (mouseY > slider.y) && (mouseY < (slider.y + slider.h)))
        {
            if (state == GuiControlState::NORMAL)
            {
                app->audio->PlayFx(5);
            }
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                if (state == GuiControlState::FOCUSED)
                {
                    app->audio->PlayFx(6);
                }
                state = GuiControlState::SELECTED; 
                if (mouseX > bounds.x && mouseX < (bounds.x + bounds.w))
                {
                    //you can move sliderbox
                    slider.x = mouseX - (slider.w * 0.5);
                }
                else if (mouseX + (int)(slider.w * 0.5)> (bounds.x + bounds.w))
                {
                    slider.x = (bounds.x + bounds.w) - slider.w;
                }
                else if ((mouseX - (int)(slider.w * 0.5))< bounds.x)
                {
                    slider.x = bounds.x;
                }
                UpdateValue();
            }

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                
                NotifyObserver();
            }
            // TODO.
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw()
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: 
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &normalSlider);
    }
        break;
    case GuiControlState::NORMAL:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &normalSlider);
    }
        break;
    case GuiControlState::FOCUSED:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &pressedBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &normalSlider);
    }
        break;
    case GuiControlState::PRESSED: 
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &normalSlider);
    }
        break;
    case GuiControlState::SELECTED: 
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &pressedBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &pressedSlider);
    }
        break;
    default:
        break;
    }

    return false;
}

void GuiSlider::SetMinValue(int val)
{
    minValue = val;
}

void GuiSlider::SetMaxValue(int val)
{
    maxValue = val;
}

void GuiSlider::SetValue(int val)
{
    value = val;
}

void GuiSlider::UpdateValue()
{
    percentValue = ((slider.x + (slider.w * 0.5)) - (bounds.x)) /
        ((bounds.x + bounds.w) - bounds.x);
    value = floor(percentValue * maxValue);
}

int GuiSlider::GetMinValue() const
{
    return minValue;
}

int GuiSlider::GetMaxValue() const
{
    return maxValue;
}

int GuiSlider::GetValue() const
{
    return value;
}

float GuiSlider::GetPercentValue() const
{
    return percentValue *100;
}

void GuiSlider::SetSlider(SDL_Rect bounds)
{
    slider = { (bounds.x + (int)(bounds.w * 0.5)) - (int)(bounds.h * 0.5), bounds.y, bounds.h,bounds.h };
    UpdateValue();
}
