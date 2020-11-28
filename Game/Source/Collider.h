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
		LEFT_WALL,
		RIGHT_WALL,
		FLOOR,
		PLAYER,
		WIN,
		WIN2,
		ROOF,
		DEATH,
		ENEMY,
	
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
