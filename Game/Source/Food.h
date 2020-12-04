#ifndef _FOOD_H_
#define _FOOD_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Food : Entity
{
public:
	Food(Module* listener, fPoint ep, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* colider);

private:
	Animation idleFood;
	Animation* currentAnimation;
};

#endif // _GROUNDENEMY_H_