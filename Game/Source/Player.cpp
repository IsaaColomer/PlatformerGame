#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "Collisions.h"
#include "Scene2.h"
#include "Intro.h"
#include "ModuleFadeToBlack.h"
#include "Scene.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	idleAnimR.speed = 0.05f;
	idleAnimR.loop = true;
	idleAnimR.PushBack({ 0,0,66,110 });
	idleAnimR.PushBack({ 72,0,66,110 });
	idleAnimR.PushBack({ 141,0,67,110 });
	//idleAnimR.PushBack({ 4,300,80,94 });

	idleAnimL.speed = 0.05f;
	idleAnimL.loop = true;
	idleAnimL.PushBack({ 0,110,67,110 });
	idleAnimL.PushBack({ 70,110,66,110 });
	idleAnimL.PushBack({ 142,110,66,110 });

	jumpAnim.speed = 0.05f;
	jumpAnim.loop = false;
	jumpAnim.PushBack({ 0,225,65,110 });
	jumpAnim.PushBack({ 72,225,57,110 });
	jumpAnim.PushBack({ 136,225,68,111 });
	jumpAnim.PushBack({ 208,225,57,110 });
	jumpAnim.PushBack({ 278,225,83,110 });

	rightAnim.speed = 0.05f;
	rightAnim.loop = true;
	rightAnim.PushBack({ 0,341,38,110  });
	rightAnim.PushBack({ 39,350,67,101 });
	rightAnim.PushBack({ 122,350,67,101 });
	rightAnim.PushBack({ 190,350,62,102 });
	rightAnim.PushBack({ 253,350,84,102 });
	rightAnim.PushBack({ 337,351,74,101 });

	leftAnim.speed = 0.05f;
	leftAnim.loop = true;
	leftAnim.PushBack({3,461,74,101});
	leftAnim.PushBack({77,460,84,102});
	leftAnim.PushBack({162,460,62,102});
	leftAnim.PushBack({225,460,67,101});
	leftAnim.PushBack({308,460,67,101});
	leftAnim.PushBack({ 376,451,38,110 });

	floppyAnim.speed = 0.05f;
	floppyAnim.loop = true;
	floppyAnim.PushBack({ 0,0,41,38 });
	floppyAnim.PushBack({ 41,0,41,38 });
	floppyAnim.PushBack({ 83,0,41,38 });
	floppyAnim.PushBack({ 126,0,41,38 });
	floppyAnim.PushBack({ 168,0,41,38 });
}

Player::~Player()
{

}

bool Player::Start()
{
	resetPlayer();
	fCount = 180;
	minusLives = false;
	collider = app->collisions->AddCollider(cp, Collider::Type::PLAYER, this);
	

	facingLeft = false;
	facingRight = true;

	winScreen = false;
	loseScreen = false;

	character = app->tex->Load("Assets/Characters/player.png");
	floppyDisk = app->tex->Load("Assets/GUI/floppy_anim.png");
	lives = app->tex->Load("Assets/GUI/heart.png");
	
	currentAnimation = &idleAnimR;
	currentFloppy = &floppyAnim;

	return true;
}

bool Player::Awake()
{
	bool ret = true;

	return ret;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		vcx = 10.0f;
	}
	else if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		vcx = 2.0f;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (facingLeft == false)
		{
			currentAnimation = &idleAnimR;
		}
		else
		{
			currentAnimation = &idleAnimL;
		}
	}
	//DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (app->intro->active)
		{
			app->fade->Fade((Module*)app->intro, (Module*)app->scene, 60);
		}
		else if (app->scene2->active)
		{
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 60);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (app->intro->active)
		{
			app->fade->Fade((Module*)app->intro, (Module*)app->scene2, 60);
		}
		else if (app->scene->active)
		{
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = (godMode) ? false : true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		cp.x = 70;
		cp.y = 500;
		vcy = 0;
		vcx = 2.0f;

		xMove = false;
		ong = false;
		app->render->camera.y = 0;
		app->render->camera.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		if (fCount == 180)
		{
			app->SaveGameRequest();
			fCount = 0;
			floppyAnim.Reset();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->collisions->DebugRequest();
	}
	//PLAYER INPUT
	if (!godMode)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ong)
		{
			vcy = -6.0f;
			ong = false;
		}
		if (!ong)
		{
			if (vcy < 6.0f) vcy -= grav;
			cp.y += vcy;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && ong == false)
		{
			if (currentAnimation != &jumpAnim)
			{
				jumpAnim.Reset();
				currentAnimation = &jumpAnim;
			}
		}
	}
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			cp.y -= 2.0f;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			cp.y += 2.0f;
		}
	}
	// L08: TODO 6: Make the camera movement independent of framerate
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cp.x -= vcx;
		if (currentAnimation != &leftAnim) {
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		if (cp.x > 640 && cp.x < 1920 && xMove)
		{
			app->render->camera.x += vcx;
		}
		xMove = true;
		facingLeft = true;
		facingRight = false;
	}
	else
	{
		ong = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cp.x += vcx;
		if (currentAnimation != &rightAnim) {
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		if (cp.x > 640 && cp.x < 1920 && xMove)
		{
			app->render->camera.x -= vcx;
		}
		xMove = true;
		facingRight = true;
		facingLeft = false;
	}
	else
	{
		ong = false;
	}
	//-----------------------COLLIDER MOVEMENT
	if (collider != nullptr)
	{
		collider->SetPos(cp.x, cp.y);
	}
	//--------------------------------

	currentAnimation->Update();
	currentFloppy->Update();
	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(character, cp.x, cp.y, &rectPlayer); // Placeholder not needed any more

	
	for (int i = 0; i < playerLives; i++)
	{
		app->render->DrawTexture(lives, -app->render->camera.x+(i*43), 0, NULL);
	}

	SDL_Rect floppyRect;
	floppyRect = currentFloppy->GetCurrentFrame();

	if (fCount < 180)
	{
		app->render->DrawTexture(floppyDisk, -app->render->camera.x, 40, &floppyRect);
		fCount++;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Player::CleanUp()
{
	active = false;
	return true;
}
bool Player::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	cp.x = play.attribute("x").as_int(0);
	cp.y = play.attribute("y").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cp.x);
	play.attribute("y").set_value(cp.y);

	if (app->scene->active == 1)
	{
		app->player->sceneValue = 1;
	}
	else if (app->scene2->active == 1)
	{
		app->player->sceneValue = 2;
	}

	pugi::xml_node scene = data.child("sceneValue");
	scene.attribute("x").set_value(sceneValue);

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && !godMode)
	{
		if (c2->type == Collider::Type::FLOOR)
		{
			ong = true;
		}
		if (c2->type == Collider::Type::LEFT_WALL)
		{
			cp.x = c2->rect.x - cp.w;
			xMove = false;
		}
		if (c2->type == Collider::Type::RIGHT_WALL)
		{
			cp.x = c2->rect.x + c2->rect.w;
			xMove = false;
		}
		if (c2->type == Collider::Type::ROOF)
		{
			cp.y = c2->rect.y + c2->rect.h;
			vcy = 0.0f;
		}
		if (c2->type == Collider::Type::WIN)
		{
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
		}
		if (c2->type == Collider::Type::WIN2)
		{
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene2, (Module*)app->titleScreen, 60);
			winScreen = true;
		}

		if (c2->type == Collider::Type::DEATH && minusLives == false)
		{
			c2->pendingToDelete = true;
			minusLives = true;
			--playerLives;
			if (playerLives == 0)
			{
				playerLives = 3;
				app->fade->Fade((Module*)app->scene2, (Module*)app->titleScreen, 60);
				loseScreen = true;
			}
			else
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->scene2, 60);
			}		
		}
		if (c2->type == Collider::Type::FOOD)
		{
			c2->pendingToDelete = true;
			if (playerLives < 3)
			{
				playerLives++;
			}
			app->scene->foodAlive = false;
			app->scene2->foodAlive = false;
		}
		if (c2->type == Collider::Type::CHECKPOINT)
		{
			c2->pendingToDelete = true;
			app->SaveGameRequest();
			app->scene->flagAlive = false;
			app->scene2->flagAlive = false;
			fCount = 0;
		}
	/*	if (c2->type == Collider::Type::ENEMY)
		{
			c2->pendingToDelete = true;
		}*/
	}
}
void Player::resetPlayer()
{
	app->player->cp.x = 80;
	app->player->cp.y = 0;
	app->player->cp.w = 66;
	app->player->cp.h = 110;
	app->player->vcy = 0;
	app->player->vcx = 2.0f;

	app->player->xMove = false;
	app->player->ong = false;

	app->render->camera.y = 0;
	app->render->camera.x = 0;
}