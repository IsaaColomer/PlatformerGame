#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"

#define GRAV -1.0f

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
	fPoint cp;
	SDL_Point savedPos;
	bool willReset;
	float vcy, vcx;
	float jump;
	bool xMove;
	bool yMove;
	bool godMode;
	bool ong;
	bool debugDraw;
	int playerLives;
	int coinsCollected;
	bool minusLives;
	bool winScreen;
	bool loseScreen;
	bool dead;
	bool notSaved;
	int jumps;
	int sceneValue;

	bool fCon;
	bool lCon;
	bool rCon;
	bool tCon;

	//AUDIO FX
	uint jumpFx = 0;
	uint checkPointFx = 0;
	uint coinFx = 0;
	uint hittedFx = 0;
	uint destroyedFx = 0;
	uint lifeUpFx = 0;
	
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
	SDL_Texture* coins;
	Collider* collider;
	Collider* colliderB;

	Collider* topCollider;
	Collider* botCollider;
	Collider* leftCollider;
	Collider* rightCollider;

	SDL_Texture* floppyDisk;
	Animation floppyAnim;
	
};

#endif //!__MODULE_PLAYER_H__