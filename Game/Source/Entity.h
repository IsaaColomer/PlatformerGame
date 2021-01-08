#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Module.h"
#include "Animation.h"
#include "Textures.h"
#include "Collider.h"

class Entity
{
public:
	enum class Type
	{
		UNKNOWN,
		ENEMYG,
		ENEMYA,
		COIN,
		FOOD,
		FLAG,
		PLAYER
		
		
	};

	Entity(Module* listener, fPoint ep, SDL_Texture* texture, Type type) : listener(listener), ep(ep), texture(texture), type(type)
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual void Collision(Collider* colider)
	{

	}

	virtual void CleanUp()
	{

	}

	Module* listener;

	Type type;
	fPoint ep;
	SDL_Texture* texture;

	Collider* collider = nullptr;

	bool pendingToDelete = false;
};

#endif // __ENTITY_H__