#include "Coin.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "Animation.h"

Coin::Coin(fPoint position, SDL_Texture* texture, Type type) : Entity(position, texture, type)
{
	idleCoin.loop = true;
	idleCoin.PushBack({ 0,0,45,45 });

	currentAnimation = &idleCoin;
}

bool Coin::Start()
{

	return true;
}

bool Coin::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Coin::Draw()
{
	SDL_Rect coinRect;
	coinRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &coinRect);

	return true;
}