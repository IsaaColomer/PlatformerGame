#include "Food.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "Animation.h"

Food::Food(fPoint position, SDL_Texture* texture, Type type) : Entity(position, texture, type)
{
	idleFood.loop = true;
	idleFood.PushBack({0,0,54,50});

	currentAnimation = &idleFood;
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
	app->render->DrawTexture(texture, position.x, position.y, &foodRect);

	return true;
}