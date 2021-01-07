#include "EnemyAir.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Audio.h"
#include "Collisions.h"
#include "Pathfinding.h"
#include "Collider.h"
#include "Map.h"

EnemyAir::EnemyAir(Module* listener, fPoint ep, SDL_Texture* texture, Type type) : Entity(listener, ep, texture, type)
{
	walkAnimLeft.speed = 5.0f;
	walkAnimLeft.loop = true;
	walkAnimLeft.PushBack({ 0,35,80,50 });
	walkAnimLeft.PushBack({ 84,37,81,48 });
	walkAnimLeft.PushBack({ 169,35,85,48 });
	walkAnimLeft.PushBack({ 258,30,80,48 });
	walkAnimLeft.PushBack({ 343,27,79,50 });
	walkAnimLeft.PushBack({ 430,24,81,58 });
	walkAnimLeft.PushBack({ 515,27,82,64 });
	walkAnimLeft.PushBack({ 606,23,82,62 });

	walkAnimRight.speed = 5.0f;
	walkAnimRight.loop = true;
	walkAnimRight.PushBack({ 608,112,80,50 });
	walkAnimRight.PushBack({ 523,114,81,48 });
	walkAnimRight.PushBack({ 434,112,85,48 });
	walkAnimRight.PushBack({ 350,107,80,48 });
	walkAnimRight.PushBack({ 266,104,79,50 });
	walkAnimRight.PushBack({ 177,101,81,58 });
	walkAnimRight.PushBack({ 91,104,82,64 });
	walkAnimRight.PushBack({ 0,100,82,62 });

	currentAnimation = &walkAnimLeft;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 80, 95 }), Collider::Type::ENEMY, listener);

	lastPathEnemy = new DynArray<fPoint>();
}

bool EnemyAir::Start()
{
	return true;
}

bool EnemyAir::Update(float dt)
{

	if (enemyAirLeft == false)
	{
		currentAnimation = &walkAnimRight;
	}
	else
	{
		currentAnimation = &walkAnimLeft;
	}
	if (app->player->cp.x < ep.x)
	{
		enemyAirLeft = true;
		enemyAirRight = false;
	}
	if (app->player->cp.x > ep.x)
	{
		enemyAirLeft = false;
		enemyAirRight = true;
	}
	if (Radar(app->player->cp))
	{
		//Direction
		/*if (ep.x < app->player->cp)
		{*/
		//If player move
		fPoint enemyPos = app->map->WorldToMap(ep.x, ep.y);
		fPoint playerPosW = app->player->cp;
		fPoint playerPosM = app->map->WorldToMap(playerPosW.x, playerPosW.y);


		//Cerate Path
		CreatePathEnemy(enemyPos, playerPosM);
		int i = GetCurrentPositionInPath(enemyPos);

		//Move Enemy
		if (lastPathEnemy->At(i + 1) != NULL)
		{
			fPoint posNextE = *lastPathEnemy->At(i + 1);
			fPoint posNextAuxE = app->map->MapToWorld(posNextE.x, posNextE.y);
			MoveEnemy(posNextAuxE, enemyPos);

			int enemyPosX = ep.x;
			int enemyPosY = ep.y;
			//MOVEMENT ON X
			if (posNextAuxE.x < enemyPosX)
			{
				ep.x -= 5;
			}
			else if (posNextAuxE.x > enemyPosX)
			{
				ep.x += 5;
			}
			//MOVEMENT ON Y
			if (app->player->cp.y < enemyPosY)
			{
				ep.y -= 5;
			}
			else if (app->player->cp.y > enemyPosY)
			{
				ep.y += 5;
			}
		}
		//}
	}
	currentAnimation->Update(dt);
	collider->SetPos(ep.x, ep.y);

	return true;
}

bool EnemyAir::Draw()
{
	SDL_Rect rectEnemy;
	rectEnemy = currentAnimation->GetCurrentFrame();
	if(!app->escaped)
		app->render->DrawTexture(texture, ep.x, ep.y, &rectEnemy);

	return true;
}

void EnemyAir::Collision(Collider* colider)
{
	if (colider->type == Collider::Type::playerBot && app->player->godMode == false)
	{
		pendingToDelete = true;
		app->audio->PlayFx(app->player->destroyedFx);
		collider->pendingToDelete = true;
	}
	if (colider->type == Collider::Type::playerLeft || colider->type == Collider::Type::playerRight || colider->type == Collider::Type::playerTop && app->player->godMode == false)
	{
		pendingToDelete = true;
		app->audio->PlayFx(app->player->hittedFx);
		collider->pendingToDelete = true;
		app->player->playerLives--;
	}
}

void EnemyAir::CleanUp()
{

}
bool EnemyAir::Radar(fPoint distance)
{
	if (ep.DistanceManhattan(distance) < range) return true;

	return false;
}
int EnemyAir::CalculateDistance(fPoint origin, fPoint destination)
{
	return abs(origin.x - destination.x) + abs(origin.y - destination.y);;
}
void EnemyAir::CreatePathEnemy(fPoint mapPositionEnemy, fPoint mapPositionDestination)
{
	if (checkDestination->check(1000))
	{
		app->pathfinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathfinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPathEnemy = app->pathfinding->GetLastPath();
	}
}
int EnemyAir::GetCurrentPositionInPath(fPoint mapPositionEnemy)
{
	int i;
	for (i = 0; i < lastPathEnemy->Count(); i++)
	{
		if (mapPositionEnemy == fPoint({ lastPathEnemy->At(i)->x, lastPathEnemy->At(i)->y }))
		{
			break;
		}
	}
	return i;
}
void EnemyAir::MoveEnemy(fPoint nextAuxPositionEenemy, fPoint mapPositionEnemy)
{
	
}