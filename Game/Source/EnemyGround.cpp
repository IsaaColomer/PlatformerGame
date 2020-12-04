#include "EnemyGround.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Collisions.h"
#include "Collider.h"

EnemyGround::EnemyGround(Module* listener, fPoint ep, SDL_Texture* texture, Type type) : Entity(listener, ep, texture, type)
{
	idleAnimation.speed = 0.05f;
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 468,593,84,93 });
	idleAnimation.PushBack({ 469,492,83,93 });
	idleAnimation.PushBack({ 470,394,79,94 });
	idleAnimation.PushBack({ 468,300,80,94 });

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 80, 95 }), Collider::Type::ENEMY, listener);
}

bool EnemyGround::Start()
{
	return true;
}

bool EnemyGround::Update(float dt)
{
	currentAnimation = &idleAnimation;
	currentAnimation->Update();
	collider->SetPos(ep.x, ep.y);

	return true;
}

bool EnemyGround::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, ep.x, ep.y, &rectEnemy);

	return true;
}

void EnemyGround::Collision(Collider* colider)
{
	if (colider->type == Collider::Type::PLAYERBOT)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
	}
	if (colider->type == Collider::Type::PLAYER)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
		app->player->playerLives--;
	}
}

void EnemyGround::CleanUp()
{

}