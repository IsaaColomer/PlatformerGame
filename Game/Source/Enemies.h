#ifndef __MODULE_ENEMIES_H__
#define __MODULE_ENEMIES_H__

#include "Module.h"
#include "Animation.h"

#define grav -0.1f


struct SDL_Texture;
struct Collider;

class Enemies : public Module
{
public:
	// Constructor
	Enemies();

	// Destructor
	~Enemies();
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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

public:

	SDL_Rect cp;
	float vcy, vcx;
	bool xMove;
	bool godMode;
	bool ong;
	bool debugDraw;
	float savedx, savedy;

	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation enemyIdleR;
	Animation enemyIdleL;
	Animation enemyJump;
	Animation enemyDown;
	Animation enemyRight;
	Animation enemyLeft;
	/*Animation shootAnim;
	Animation death;*/

	bool facingLeft;
	bool idleState;
	bool facingRight;

	SDL_Texture* firstEnemy;
	Collider* collider;
};

#endif //!__MODULE_PLAYER_H__