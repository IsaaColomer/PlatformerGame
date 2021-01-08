#include "Flag.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"
#include "Animation.h"
#include "Audio.h"
#include "Scene.h"
#include "Scene2.h"
#include "Scene3.h"

Flag::Flag(Module * listener, fPoint ep, SDL_Texture * texture, Type type) : Entity(listener, ep, texture, type)
{
	idleFlag.loop = true;
	idleFlag.PushBack({ 0,11,33,169 });

	hittedFlag.loop = true;
	hittedFlag.PushBack({ 74,11,33,169 });

	hittedFx = app->audio->LoadFx("Assets/Audio/Fx/hitted.wav");

	currentAnimation = &idleFlag;
	//{ 900, 100, 10, 170 };
	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 33, 169 }), Collider::Type::COIN, listener);
}

bool Flag::Start()
{

	return true;
}

bool Flag::Update(float dt)
{

	currentAnimation->Update(dt);
	if (app->scene3->scene3OnScreen)
		pendingToDelete = true;
	return true;
}

bool Flag::Draw()
{
	SDL_Rect flagRect;
	flagRect = currentAnimation->GetCurrentFrame();
	if (app->escaped == false)
		app->render->DrawTexture(texture, ep.x, ep.y, &flagRect);

	return true;
}

void Flag::Collision(Collider* coll)
{
	if ((coll == app->player->topCollider || coll == app->player->rightCollider || coll == app->player->leftCollider || coll == app->player->botCollider) && app->player->godMode == false)
	{
		if (app->player->ong)
		{
			currentAnimation = &hittedFlag;
			app->audio->PlayFx(hittedFx);
			collider->pendingToDelete = true;
			app->scene->flagAlive = false;
			app->scene2->flagAlive = false;
			app->SaveGameRequest();
			app->player->fCount = 0;

		//	pendingToDelete = true;
		}
	}
}
