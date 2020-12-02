#ifndef _FOOD_H_
#define _FOOD_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Textures.h"

struct SDL_Texture;
struct Collider;

class Food : Entity
{
public:
	Food(fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

private:
	Animation idleFood;
	Animation* currentAnimation;
};

#endif // _GROUNDENEMY_H_