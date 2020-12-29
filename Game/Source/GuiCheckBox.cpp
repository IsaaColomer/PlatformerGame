#include "GuiCheckBox.h"
#include "App.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (checked) render->DrawRectangle(bounds, 100, 100, 100, 255);
        else render->DrawRectangle(bounds, 100, 100, 100, 255);
    } break;
    case GuiControlState::NORMAL: 
    {
        if (checked) app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
        else app->render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
    } break;
    case GuiControlState::FOCUSED: app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
        break;
    case GuiControlState::PRESSED: app->render->DrawTexture(textureClicked, bounds.x, bounds.y, NULL);
        break;
    case GuiControlState::SELECTED: app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
        break;
    default:
        break;
    }

    return false;
}
