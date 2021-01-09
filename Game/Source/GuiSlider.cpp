#include "GuiSlider.h"

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
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
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
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        app->render->DrawRectangle(slider, 100, 255, 100, 255);
    }
        break;
    case GuiControlState::NORMAL:
    {
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        app->render->DrawRectangle(slider, 155, 255, 0, 255);
    }
        break;
    case GuiControlState::FOCUSED:
    {
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        app->render->DrawRectangle(slider, 255, 255, 100, 255);
    }
        break;
    case GuiControlState::PRESSED: 
    {
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        app->render->DrawRectangle(slider, 125, 255, 255, 255);
    }
        break;
    case GuiControlState::SELECTED: 
    {
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        app->render->DrawRectangle(slider, 125, 255, 0, 255);
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

int GuiSlider::GetMinValue() const
{
    return minValue;
}

int GuiSlider::GetMaxValue() const
{
    return maxValue;
}

void GuiSlider::SetSlider(SDL_Rect bounds)
{
    slider = { (bounds.x + (int)(bounds.w * 0.5)) - (int)(bounds.h * 0.5), bounds.y, bounds.h,bounds.h };
}
