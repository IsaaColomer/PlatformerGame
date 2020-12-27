#ifndef __COLLIDER_H__
#define __COLLIDER_H__
#include "SDL/include/SDL_Rect.h"
#define MAX_LISTENERS 1000

class Module;
struct Collider
{
	enum Type
	{
		NONE = -1,
		playerLeft,
		playerRight,
		COLL,
		playerBot,
		WIN,
		WIN2,
		WIN3,
		playerTop,
		DEATH,
		ENEMY,
		PLAYERBOT,
		FOOD,
		COIN,
		CHECKPOINT,
	
		MAX
	};

	enum Height
	{
		UNDEFINED
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);
	bool Intersects(const SDL_Rect& r) const;
	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	Type type;
	Height height;

	bool pendingToDelete = false;

	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__
