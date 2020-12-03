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
		COIN,
		FOOD
		
	};

	Entity (fPoint position, SDL_Texture* texture,Collider* collider, Type type) : position(position), texture(texture),collider(collider), type(type)
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

	Type type;
	fPoint position;
	SDL_Texture* texture;
	Collider* collider;

	bool pendingToDelete = false;
};

#endif // __ENTITY_H__