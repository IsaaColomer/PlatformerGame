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
#include "FadeToBlack.h"
#include "Scene.h"
#include "Scene3.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	idleAnimR.speed = 0.08f;
	idleAnimR.loop = true;
	idleAnimR.PushBack({ 0,0,66,110 });
	idleAnimR.PushBack({ 72,0,66,110 });
	idleAnimR.PushBack({ 141,0,67,110 });
	//idleAnimR.PushBack({ 4,300,80,94 });

	idleAnimL.speed = 0.08f;
	idleAnimL.loop = true;
	idleAnimL.PushBack({ 0,110,67,110 });
	idleAnimL.PushBack({ 70,110,66,110 });
	idleAnimL.PushBack({ 142,110,66,110 });

	jumpAnim.speed = 0.08f;
	jumpAnim.loop = false;
	jumpAnim.PushBack({ 0,225,65,110 });
	jumpAnim.PushBack({ 72,225,57,110 });
	jumpAnim.PushBack({ 136,225,68,111 });
	jumpAnim.PushBack({ 208,225,57,110 });
	jumpAnim.PushBack({ 278,225,83,110 });

	rightAnim.speed = 0.08f;
	rightAnim.loop = true;
	rightAnim.PushBack({ 0,341,38,110  });
	rightAnim.PushBack({ 39,350,67,101 });
	rightAnim.PushBack({ 122,350,67,101 });
	rightAnim.PushBack({ 190,350,62,102 });
	rightAnim.PushBack({ 253,350,84,102 });
	rightAnim.PushBack({ 337,351,74,101 });

	leftAnim.speed = 0.08f;
	leftAnim.loop = true;
	leftAnim.PushBack({3,461,74,101});
	leftAnim.PushBack({77,460,84,102});
	leftAnim.PushBack({162,460,62,102});
	leftAnim.PushBack({225,460,67,101});
	leftAnim.PushBack({308,460,67,101});
	leftAnim.PushBack({ 376,451,38,110 });

	floppyAnim.speed = 0.12f;
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
	
	facingLeft = false;
	facingRight = true;

	fCon = false;
	lCon = false;
	rCon = false;
	tCon = false;

	winScreen = false;
	loseScreen = false;

	notSaved = false;

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	dead = false;
	savedPos = { 80,0 };

	character = app->tex->Load("Assets/Characters/player.png");
	floppyDisk = app->tex->Load("Assets/GUI/floppy_anim.png");
	lives = app->tex->Load("Assets/GUI/heart.png");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	checkPoint = app->audio->LoadFx("Assets/Audio/Fx/check_point.wav");

	collider = app->collisions->AddCollider(cp, Collider::Type::PLAYER, this);

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
		else if (app->scene3->active)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->scene2, 60);
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
		else if (app->scene3->active)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->scene2, 60);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (app->intro->active)
		{
			app->fade->Fade((Module*)app->intro, (Module*)app->scene3, 60);
		}
		else if (app->scene->active)
		{
			app->fade->Fade((Module*)app->scene, (Module*)app->scene3, 60);
		}
		else if (app->scene2->active)
		{
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene3, 60);
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
		vcx = 290.0f;

		xMove = false;
		ong = false;
		app->render->camera.y = 0;
		app->render->camera.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		if (app->scene->active == true && app->scene2->flagAlive == false && app->scene->flagAlive == true)
		{
			//SCENE 2
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 1);
			resetPlayer();
		}
		if (app->scene2->active == true && app->scene->flagAlive == false && app->scene2->flagAlive == true)
		{
			//SCENE 1
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 1);
			resetPlayer();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		if (fCount == 180)
		{
			app->SaveGameRequest();
			fCount = 0;
			floppyAnim.Reset();
		}
		notSaved = true;
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
			vcy = -650.0;
			ong = false;
			app->audio->PlayFx(jumpFx);
		}
		if (!ong)
		{
			if (vcy < 300) vcy -= grav*dt;
			cp.y += vcy*dt;
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
		if (rCon == false)
		{
			cp.x -= vcx * dt;
			if (cp.x > 640 && cp.x < 1920 && xMove)
			{
				app->render->camera.x += vcx * dt;
			}
		}
		else
		{
			rCon = false;
		}
		if (currentAnimation != &leftAnim) {
			leftAnim.Reset();
			currentAnimation = &leftAnim;
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
		if (lCon == false)
		{
			cp.x += vcx * dt;
			if (cp.x > 640 && cp.x < 1920 && xMove)
			{
				app->render->camera.x -= vcx * dt;
			}
		}
		else
		{
			lCon = false;
		}

		if (currentAnimation != &rightAnim) {
			rightAnim.Reset();
			currentAnimation = &rightAnim;
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

	if (willReset)
	{
		resetPlayer();
	}

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
	LOG("Freeing player");
	app->tex->UnLoad(character);
	app->tex->UnLoad(floppyDisk);
	app->tex->UnLoad(lives);

	app->player->active = false;
	return true;
}
bool Player::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	cp.x = play.attribute("x").as_int(0);
	cp.y = play.attribute("y").as_int(0);

	app->player->savedPos.x = cp.x;
	app->player->savedPos.y = cp.y;

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cp.x);
	play.attribute("y").set_value(cp.y);

	app->player->savedPos.x = cp.x;
	app->player->savedPos.y = cp.y;

	pugi::xml_node scene = data.child("sceneValue");
	scene.attribute("x").set_value(sceneValue);

	if (app->scene->active == 1)
	{
		app->player->sceneValue = 1;
	}
	else if (app->scene2->active == 1)
	{
		app->player->sceneValue = 2;
	}
	else if (app->scene3->active == 1)
	{
		app->player->sceneValue = 3;
	}
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && !godMode)
	{
		if (c2->type == Collider::Type::FLOOR)
		{
			ong = true;
			fCon = true;
			minusLives = false;
		}
		if (c2->type == Collider::Type::LEFT_WALL && lCon == false)
		{
			cp.x = c2->rect.x - cp.w+1;
			lCon = true;
			xMove = false;
		}
		if (c2->type == Collider::Type::RIGHT_WALL && rCon == false)
		{
			cp.x = c2->rect.x + c2->rect.w-1;
			rCon = true;
			xMove = false;
		}
		if (c2->type == Collider::Type::ROOF)
		{
			cp.y = c2->rect.y + c2->rect.h;
			tCon = true;
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
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene3, 60);
		}
		if (c2->type == Collider::Type::WIN3)
		{
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene3, (Module*)app->titleScreen, 60);
			winScreen = true;
		}
		if (c2->type == Collider::Type::DEATH && minusLives == false)
		{
			c2->pendingToDelete = true;
			dead = true;
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
				willReset = true;
				//app->fade->Fade((Module*)app->scene2, (Module*)app->scene2, 60);
			}
		}
		if (c2->type == Collider::Type::FOOD)
		{
			c2->pendingToDelete = true;
			if (playerLives < 3)
			{
				playerLives++;
			}
			/*for (int i = 0; i < MAX_COLLECTIBLES; i++)
			{
				app->collectible->foodAlive = false;
			}
			app->scene2->foodAlive = false;*/
		}
		if (c2->type == Collider::Type::CHECKPOINT)
		{
			c2->pendingToDelete = true;
			app->audio->PlayFx(checkPoint);
			app->SaveGameRequest();
			app->scene->flagAlive = false;
			app->scene2->flagAlive = false;
			fCount = 0;
		}
		if (c2->type == Collider::Type::COIN)
		{
			c2->pendingToDelete = true;
		}
	}
}
void Player::resetPlayer()
{
	willReset = false;
	if (app->scene->active == true)
	{
		app->player->cp.x = 80;
		app->player->cp.y = 0;
	}
	if (app->scene2->active == true)
	{
		app->player->cp.x = 80;
		app->player->cp.y = 0;
	}
	if (app->scene3->active == true)
	{
		app->player->cp.x = 0;
		app->player->cp.y = 0;
	}
	app->player->cp.w = 66;
	app->player->cp.h = 110;

	app->player->vcy = 0;
	app->player->vcx = 290;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->player->xMove = false;
	app->player->ong = false;
}