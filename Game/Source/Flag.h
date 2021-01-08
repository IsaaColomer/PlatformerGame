#ifndef _FLAG_H_
#define _FLAG_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Textures.h"

struct SDL_Texture;
struct Collider;

class Flag : Entity
{
public:
	Flag(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* colider);

private:
	Animation idleFlag;
	Animation hittedFlag;
	Animation* currentAnimation;

	uint hittedFx = 0;
};

#endif // _GROUNDENEMY_H_