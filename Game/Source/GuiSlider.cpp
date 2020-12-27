#include "GuiSlider.h"
#include "App.h"
#include "ConfigScene.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        int motionX, motionY;
        input->GetMouseMotion(motionX, motionY);
        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                if (bounds.x+bounds.w >= app->configscene->fxSliderBack.x)
                {
                    bounds.x = mouseX - (bounds.w / 2);  // Slider
                    if (bounds.x + bounds.w <= app->configscene->fxSliderBack.x + app->configscene->fxSliderBack.w)
                    {
                        bounds.x = mouseX - (bounds.w / 2);  // Slider
                    }
                    else
                    {
                        bounds.x = app->configscene->fxSliderBack.x + app->configscene->fxSliderBack.w - bounds.w;
                    }
                }
                else
                {
                    bounds.x = app->configscene->fxSliderBack.x;
                }
               

                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
  
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL:   app->render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::FOCUSED: app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::PRESSED: app->render->DrawTexture(textureClicked, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        default:
            break;
        }
    return false;
}
