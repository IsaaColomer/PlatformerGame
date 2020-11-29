#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"

#define grav -0.1f


struct SDL_Texture;
struct Collider;

class Player : public Module
{
public:
	// Constructor
	Player();

	// Destructor
	~Player();
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

	void resetPlayer();

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
	int playerLives = 3;
	bool minusLives;
	//float coll[11][3] =
	//{
	//	700,0,2520,
	//	560,280,490,
	//	420,560,630,
	//	280,770,980,
	//	490,980,1260,
	//	350,1330,1610,
	//	280,1680,1750,
	//	350,1890,1960,
	//	420,2030,2100,
	//	490,2170,2240,
	//	420,2310,2450
	//};
	
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnimR;
	Animation idleAnimL;
	Animation jumpAnim;
	Animation downAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation shootAnim;
	Animation death;

	bool facingLeft;
	bool idleState;
	bool facingRight;
	int fCount;

	SDL_Texture* character;
	SDL_Texture* floppyDisk;
	SDL_Texture* lives;
	Collider* collider;
};

#endif //!__MODULE_PLAYER_H__