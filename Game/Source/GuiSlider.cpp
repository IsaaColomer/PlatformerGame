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
        SDL_Point mouse;
        input->GetMousePosition(mouse.x, mouse.y);

        SDL_Point motion;
        input->GetMouseMotion(motion.x, motion.y);

        if (SDL_PointInRect(&mouse, &bounds))
        {
            state = GuiControlState::FOCUSED;
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;

                bounds.x = mouse.x - (bounds.w / 2);
                int left = app->configscene->musicSliderBack.x + 1;
                if (bounds.x < left)
                    bounds.x = left;
                int right = app->configscene->musicSliderBack.x + app->configscene->musicSliderBack.w - 1 - bounds.w;
                if (bounds.x > right)
                    bounds.x = right;

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
