#ifndef _GROUNDENEMY_H_
#define _GROUNDENEMY_H_

#include "Point.h"
#include "Entity.h"
#include "DynArray.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;
struct Collider;

class EnemyGround : Entity
{
public:
	EnemyGround(Module* listener, fPoint position, SDL_Texture* texture, Type type);

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

	int range = 600;
	bool isDetected = false;
	bool returning;

	float vey;
	float gravity = 0.2f;
	float vex = 0.0f;

	Timer* checkDestination = new Timer();
	fPoint destination;
	fPoint positionInitial;

	DynArray<fPoint>* lastPathEnemy;
};

#endif // _GROUNDENEMY_H_