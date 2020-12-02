#ifndef _COIN_H_
#define _COIN_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Textures.h"

struct SDL_Texture;
struct Collider;

class Coin : Entity
{
public:
	Coin(fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

private:
	Animation idleCoin;
	Animation* currentAnimation;
};

#endif // _GROUNDENEMY_H_