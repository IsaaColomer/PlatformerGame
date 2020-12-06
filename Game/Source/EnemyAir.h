#ifndef _ENEMYAIR_H_
#define _ENEMYAIR_H_

#include "Point.h"
#include "Entity.h"
#include "DynArray.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;
struct Collider;

class EnemyAir : Entity
{
public:
	EnemyAir(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* colider);

	void CleanUp();

private:
	Animation idleAnimation;
	Animation walkAnimRight;
	Animation* currentAnimation;

	bool Radar(fPoint origin);
	void CreatePathEnemy(fPoint origin, fPoint destination);
	int GetCurrentPositionInPath(fPoint mapPositionEnemy);
	void MoveEnemy(fPoint nextAuxPositionEenemy, fPoint mapPositionEnemy);
	void MoveEnemyNULL(fPoint mapPositionEnemy);
	int CalculateDistance(fPoint origin, fPoint destination);

	int range = 700;
	bool isDetected = false;
	bool returning;

	Timer* checkDestination = new Timer();
	fPoint destination;
	fPoint positionInitial;

	DynArray<fPoint>* lastPathEnemy;
};

#endif // _GROUNDENEMY_H_