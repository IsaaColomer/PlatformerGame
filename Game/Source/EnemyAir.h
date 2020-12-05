#ifndef _ENEMYAIR_H_
#define _ENEMYAIR_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

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
};

#endif // _GROUNDENEMY_H_