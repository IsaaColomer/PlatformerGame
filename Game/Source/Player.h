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
	bool winScreen;
	bool loseScreen;
	int sceneValue;
	
	Animation* currentAnimation = nullptr;
	Animation* currentFloppy = nullptr;

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
	SDL_Texture* lives;
	Collider* collider;

	SDL_Texture* floppyDisk;
	Animation floppyAnim;
	
};

#endif //!__MODULE_PLAYER_H__