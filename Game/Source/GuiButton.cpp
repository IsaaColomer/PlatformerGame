#include "GuiButton.h"
#include "App.h"
#include "Intro.h"
#include "ConfigScene.h"
#include "Audio.h"
#include "Player.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    audioFx = false;
    pressedFx = false;
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/selected.wav");
    pressedFx = app->audio->LoadFx("Assets/Audio/Fx/clicked.wav");
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
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
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawTexture(textureDisabled, bounds.x, bounds.y, NULL);
        break;
    case GuiControlState::NORMAL:
        if(app->intro->debug || app->configscene->debug)
        {
            render->DrawRectangle(bounds, 0, 255, 0, 255);
        }
        if (app->intro->debug == false || app->configscene->debug==false)
        {
            app->render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
        }
        audioFx = false;
        pressedFxB = false;
        break;
    case GuiControlState::FOCUSED: 
        if (app->intro->debug || app->configscene->debug == true)
        {
            render->DrawRectangle(bounds, 255, 0, 0, 255);
        }
        if (app->intro->debug == false || app->configscene->debug == false)
        {
            app->render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
        }
        if (audioFx == false)
        {
            audioFx = true;
            app->audio->PlayFx(buttonFx);
        }
        break;
    case GuiControlState::PRESSED:
        if (app->intro->debug || app->configscene->debug == true)
        {
            render->DrawRectangle(bounds, 0, 0, 255, 255);
        }
        if (app->intro->debug == false || app->configscene->debug == false)
        {
            app->render->DrawTexture(textureClicked, bounds.x, bounds.y, NULL);
        }
        if (pressedFxB == false)
        {
            pressedFxB = true;
            app->audio->PlayFx(pressedFx);
        }
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }

    return false;
}
