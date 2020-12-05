#include "Food.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "Animation.h"
#include "Audio.h"

Food::Food(Module* listener, fPoint ep, SDL_Texture* texture, Type type) : Entity(listener, ep, texture, type)
{
	idleFood.loop = true;
	idleFood.PushBack({0,0,54,50});

	currentAnimation = &idleFood;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 54, 50 }), Collider::Type::FOOD, listener);
}

bool Food::Start()
{

	return true;
}

bool Food::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Food::Draw()
{
	SDL_Rect foodRect;
	foodRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, ep.x, ep.y, &foodRect);

	return true;
}

void Food::Collision(Collider* colider)
{
	collider->pendingToDelete = true;

	if (app->player->playerLives < 3 && app->player->godMode == false)
	{
		app->player->playerLives++;
		app->audio->PlayFx(app->player->lifeUpFx);
	}
	pendingToDelete = true;
}