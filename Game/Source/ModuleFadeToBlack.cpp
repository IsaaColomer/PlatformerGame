#include "ModuleFadeToBlack.h"

#include "Window.h"
#include "App.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack() : Module()
{
	name = "fade";

	//corregir
	screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update()
{
	// Exit this function if we are not performing a fade
	//if (currentStep == Fade_Step::NONE) return Update_Status::UPDATE_CONTINUE;

	bool ret = true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			currentStep = Fade_Step::FROM_BLACK;

			if (moduleToDisable->active)
			{
				moduleToDisable->CleanUp();
			}
			if (moduleToEnable->active == false)
			{
				moduleToEnable->Init();
				moduleToEnable->Start();
			}
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return ret;
}

bool FadeToBlack::PostUpdate()
{
	bool ret = true;
	// Exit this function if we are not performing a fade
//	if (currentStep == Fade_Step::NONE) return Update_Status::UPDATE_CONTINUE;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return ret;
}

bool FadeToBlack::Fade(Module* toDisable, Module* toEnable, float frames)
{
	bool ret = false;

	moduleToDisable = toDisable;
	moduleToEnable = toEnable;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		maxFadeFrames = frames;

		ret = true;
	}

	frameCount = 0;
	return ret;
}