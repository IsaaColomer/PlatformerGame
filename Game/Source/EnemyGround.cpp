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
	idleAnimationR.speed = 0.05f;
	idleAnimationR.loop = true;
	idleAnimationR.PushBack({ 468,593,84,93 });
	idleAnimationR.PushBack({ 469,492,83,93 });
	idleAnimationR.PushBack({ 470,394,79,94 });
	idleAnimationR.PushBack({ 468,300,80,94 });

	idleAnimationL.speed = 0.05f;
	idleAnimationL.loop = true;
	idleAnimationL.PushBack({ 0,593,84,93 });
	idleAnimationL.PushBack({ 0,492,83,93 });
	idleAnimationL.PushBack({ 3,394,79,94 });
	idleAnimationL.PushBack({ 4,300,80,94 });

	walkAnimRight.speed = 0.05f;
	walkAnimRight.loop = true;
	walkAnimRight.PushBack({ 243,592,75,94 });
	walkAnimRight.PushBack({ 249,492,69,93 });
	walkAnimRight.PushBack({ 249,394,77,94 });
	walkAnimRight.PushBack({ 243,292,75,94 });
	walkAnimRight.PushBack({ 248,187,70,94 });
	walkAnimRight.PushBack({ 251,80,67,94 });

	walkAnimLeft.speed = 0.05f;
	walkAnimLeft.loop = true;
	walkAnimLeft.PushBack({ 358,592,75,94 });
	walkAnimLeft.PushBack({ 358,492,69,93 });
	walkAnimLeft.PushBack({ 350,394,77,94 });
	walkAnimLeft.PushBack({ 358,292,75,94 });
	walkAnimLeft.PushBack({ 358,187,70,94 });
	walkAnimLeft.PushBack({ 358,80,67,94 });



	currentAnimation = &idleAnimationR;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)ep.x, (int)ep.y, 80, 95 }), Collider::Type::ENEMY, listener);

	lastPathEnemy = new DynArray<fPoint>();
}

bool EnemyGround::Start()
{
	enemyLeft = false;
	enemyRight = true;
	return true;
}

bool EnemyGround::Update(float dt)
{
	if (enemyLeft == false)
	{
		currentAnimation = &idleAnimationL;
	}
	else
	{
		currentAnimation = &idleAnimationR;
	}
	if (app->player->cp.x < ep.x)
	{
		enemyLeft = true;
		enemyRight = false;
	}
	if (app->player->cp.x > ep.x)
	{
		enemyLeft = false;
		enemyRight = true;
	}
	if (app->pathfinding->destinationIsFind == false && enemyRight == true)
	{
		currentAnimation = &walkAnimRight;
	}
	if (app->pathfinding->destinationIsFind == false && enemyLeft == true)
	{
		currentAnimation = &walkAnimLeft;
	}
	if (app->pathfinding->destinationIsFind == true && enemyRight == true)
	{
		currentAnimation = &idleAnimationR;
	}
	if (app->pathfinding->destinationIsFind == true && enemyRight == true)
	{
		currentAnimation = &idleAnimationL;
	}
	vey += gravity;
	ep.x += vex;
	ep.y += vey;

	if (Radar(app->player->cp))
	{
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
			}
		//}
	}
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
	if (colider->type == Collider::Type::playerBot && app->player->godMode == false)
	{
		pendingToDelete = true;
		app->audio->PlayFx(app->player->destroyedFx);
		collider->pendingToDelete = true;
	}
	if ((colider->type == Collider::Type::playerTop || colider->type == Collider::Type::playerLeft || colider->type == Collider::Type::playerRight) && app->player->godMode == false)
	{
		app->player->playerLives--;
		pendingToDelete = true;
		app->audio->PlayFx(app->player->hittedFx);
		collider->pendingToDelete = true;
	}
	if (colider->type == Collider::Type::COLL)
	{
		ep.y = colider->rect.y - 93;
		vey = 0;
		ep.y = ep.y;
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
void EnemyGround::MoveEnemy(fPoint nextPosAuxE, fPoint mapPositionEnemy)
{
	int positionEnemyX = ep.x;
	int positionEnemyY = ep.y;
//	int velocity = velocity;
		if (nextPosAuxE.x < positionEnemyX)
		{
			ep.x -= 5;
		}
		else if (nextPosAuxE.x > positionEnemyX)
		{
			ep.x += 5;
		}
}

