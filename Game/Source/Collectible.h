#ifndef __COLLECTIBLE_H__
#define __COLLECTIBLE_H__

#include "Module.h"
#include "Animation.h"

#define MAX_COLLECTIBLES 5

struct SDL_Texture;
struct Collider;

class Collectible : public Module
{
public:
	// Constructor
	Collectible();

	// Destructor
	~Collectible();
	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2);

	//void DebugDrawGamepadInfo();

private:
	SDL_Texture* food;
	SDL_Texture* coin;
	bool destroy[MAX_COLLECTIBLES];
	Collider* collectibleCol[MAX_COLLECTIBLES];
	SDL_Rect collectibleRect[MAX_COLLECTIBLES];
	Collider::Type collectibleTypes[MAX_COLLECTIBLES];
};

#endif //!__MODULE_PLAYER_H__