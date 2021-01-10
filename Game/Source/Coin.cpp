#include "Coin.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "Animation.h"
#include "Audio.h"

Coin::Coin(Module* listener, fPoint ep, SDL_Texture* texture, Type type) : Entity(listener, ep, texture, type)
{
	idleCoin.loop = true;
	idleCoin.PushBack({ 0,0,45,45 });

	currentAnimation = &idleCoin;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 45, 45 }), Collider::Type::COIN, listener);
}

bool Coin::Start()
{

	return true;
}

bool Coin::Update(float dt)
{
	currentAnimation->Update(dt);
	return true;
}

bool Coin::Draw()
{
	SDL_Rect coinRect;
	coinRect = currentAnimation->GetCurrentFrame();
	if(app->escaped == false)
		app->render->DrawTexture(texture, ep.x, ep.y, &coinRect);

	return true;
}

void Coin::Collision(Collider* coll)
{
	if ((coll == app->player->topCollider || coll == app->player->rightCollider || coll == app->player->leftCollider || coll == app->player->botCollider) && app->player->godMode == false)
	{
		collider->pendingToDelete = true;
		if (app->player->coinsCollected >= 0 || app->player->coinsCollected <= 10)
		{
			app->player->coinsCollected++;
		}
		app->audio->PlayFx(app->player->coinFx);
		pendingToDelete = true;
	}
	
}