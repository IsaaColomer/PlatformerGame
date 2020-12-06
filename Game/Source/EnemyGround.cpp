#include "EnemyGround.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Audio.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "Collider.h"
#include "Map.h"
#include "DynArray.h"
#include "Timer.h"

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

	lastPathEnemy = new DynArray<fPoint>();
}

bool EnemyGround::Start()
{
	return true;
}

bool EnemyGround::Update(float dt)
{
	if (Radar(app->player->cp))
	{
		//Direction
		/*if (ep.x < app->player->cp)
		{*/
			//If player move
			fPoint mapPositionEnemy = app->map->WorldToMap(ep.x, ep.y);
			fPoint worldPositionPalyer = app->player->cp;
			fPoint mapPositionPalyer = app->map->WorldToMap(worldPositionPalyer.x, worldPositionPalyer.y);


			//Cerate Path
			CreatePathEnemy(mapPositionEnemy, mapPositionPalyer);
			int i = GetCurrentPositionInPath(mapPositionEnemy);

			//Move Enemy
			if (lastPathEnemy->At(i + 1) != NULL)
			{
				fPoint nextPositionEnemy = *lastPathEnemy->At(i + 1);
				fPoint nextAuxPositionEenemy = app->map->MapToWorld(nextPositionEnemy.x, nextPositionEnemy.y);
				MoveEnemy(nextAuxPositionEenemy, mapPositionEnemy);
			}
		//}
	}
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
	if (colider->type == Collider::Type::PLAYERBOT && app->player->godMode == false)
	{
		pendingToDelete = true;
		app->audio->PlayFx(app->player->destroyedFx);
		collider->pendingToDelete = true;
	}
	if (colider->type == Collider::Type::PLAYER && app->player->godMode == false)
	{
		pendingToDelete = true;
		app->audio->PlayFx(app->player->hittedFx);
		collider->pendingToDelete = true;
		app->player->playerLives--;
	}
}

void EnemyGround::CleanUp()
{

}

bool EnemyGround::Radar(fPoint distance)
{
	if (ep.DistanceManhattan(distance) < range) return true;

	return false;
}
int EnemyGround::CalculateDistance(fPoint origin, fPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}
void EnemyGround::CreatePathEnemy(fPoint mapPositionEnemy, fPoint mapPositionDestination)
{
	if (checkDestination->check(1000))
	{
		app->pathfinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathfinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPathEnemy = app->pathfinding->GetLastPath();
	}
}
int EnemyGround::GetCurrentPositionInPath(fPoint mapPositionEnemy)
{
	int i;
	for (i = 0; i < lastPathEnemy->Count(); i++)
	{
		if (mapPositionEnemy == fPoint({ lastPathEnemy->At(i)->x, lastPathEnemy->At(i)->y })) break;
	}
	return i;
}
void EnemyGround::MoveEnemy(fPoint nextAuxPositionEenemy, fPoint mapPositionEnemy)
{
	int positionEnemyX = ep.x;
	int positionEnemyY = ep.y;
//	int velocity = velocity;
		if (nextAuxPositionEenemy.x < positionEnemyX)
		{
			ep.x -= 5;
		}
		else if (nextAuxPositionEenemy.x > positionEnemyX)
		{
			ep.x += 5;
		}
}

