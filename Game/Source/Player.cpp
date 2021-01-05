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
#include "EntityManager.h"

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

	playerLives = 3;
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
	jumps = 0;

	//ALL TEXTURES AND STUFF TO LOAD (AUDIO, PNG'S, etc...)
	character = app->tex->Load("Assets/Characters/player.png");
	floppyDisk = app->tex->Load("Assets/GUI/floppy_anim.png");
	lives = app->tex->Load("Assets/GUI/heart.png");
	coins = app->tex->Load("Assets/GUI/coin.png");
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	coinFx = app->audio->LoadFx("Assets/Audio/Fx/coin.wav");
	hittedFx = app->audio->LoadFx("Assets/Audio/Fx/hitted.wav");
	checkPointFx = app->audio->LoadFx("Assets/Audio/Fx/check_point.wav");
	destroyedFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_destroyed.wav");
	lifeUpFx = app->audio->LoadFx("Assets/Audio/Fx/life.wav");

	SDL_Rect top = { cp.x + 10,cp.y, 66 - 20, 10 };
	SDL_Rect bot = { cp.x + 10,cp.y + 110, 66 - 20, 10 };
	SDL_Rect left = { cp.x,cp.y + 10, 10, 110 - 20 };
	SDL_Rect right = { cp.x + 66 - 10,cp.y + 10, 10, 110 - 20};
	topCollider = app->collisions->AddCollider(top, Collider::Type::playerTop, this);
	botCollider = app->collisions->AddCollider(bot, Collider::Type::playerBot, this);
	leftCollider = app->collisions->AddCollider(left, Collider::Type::playerRight, this);
	rightCollider = app->collisions->AddCollider(right, Collider::Type::playerLeft, this);

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
			app->entitymanager->CleanUp();
		}
		else if (app->scene3->active)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->scene, 60);
			app->entitymanager->CleanUp();
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
			app->entitymanager->CleanUp();
		}
		else if (app->scene3->active)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->scene2, 60);
			app->entitymanager->CleanUp();
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
			app->entitymanager->CleanUp();
		}
		else if (app->scene2->active)
		{
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene3, 60);
			app->entitymanager->CleanUp();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		resetPlayer();
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->SaveGameRequest();
		if (sceneOneA)
		{
			app->fade->Fade((Module*)app->scene, (Module*)app->scenepause, 10);
		}
		if (sceneTwoA == true)
		{
			app->fade->Fade((Module*)app->scene2, (Module*)app->scenepause, 10);
		}
		if (sceneThreeA == true)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->scenepause, 10);
		}
		
		app->intro->gamePaused = true;
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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps < 2)
		{
			vcy = jump;
			app->audio->PlayFx(jumpFx);
			ong = false;
			jumps++;
		}
		
		if (!ong)
		{
			if (vcy < 500) vcy -= GRAV * dt;
			cp.y += vcy * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
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
		cp.x -= vcx * dt;
		ong = false;
		if (currentAnimation != &leftAnim)
		{
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		facingLeft = true;
		facingRight = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cp.x += vcx * dt;
		ong = false;
		if (currentAnimation != &rightAnim)
		{
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		facingRight = true;
		facingLeft = false;
	}
	//-----------------------COLLIDER MOVEMENT

	if (topCollider != nullptr && botCollider != nullptr && rightCollider != nullptr && leftCollider != nullptr)
	{
		topCollider->SetPos(cp.x + 10, cp.y);
		botCollider->SetPos(cp.x + 10, cp.y + 110 - 10);
		rightCollider->SetPos(cp.x, cp.y + 10);
		leftCollider->SetPos(cp.x + 66 - 10, cp.y + 10);
	}
	//--------------------------------

	//-----------------------CAMERA MOVEMENT
	if (cp.x > 640 && cp.x < 1920)
	{
		app->render->camera.x = -(cp.x - 640);
	}

	if (playerLives == 0)
	{
		if (sceneOneA == true)
		{
			app->fade->Fade((Module*)app->scene, (Module*)app->titleScreen, 60);
		}
		if (sceneTwoA == true)
		{
			app->fade->Fade((Module*)app->scene2, (Module*)app->titleScreen, 60);
		}
		if (sceneThreeA == true)
		{
			app->fade->Fade((Module*)app->scene3, (Module*)app->titleScreen, 60);
		}
		loseScreen = true;
		playerLives = 3;
	}

	currentAnimation->Update();
	currentFloppy->Update();

	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;

	SDL_Rect rectPlayer = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(character, cp.x, cp.y, &rectPlayer);
	
	for (int i = 0; i < playerLives; i++)
	{
		app->render->DrawTexture(lives, -app->render->camera.x+(i*43), 0, NULL);
	}
	for (int i = 0; i < coinsCollected; i++)
	{
		app->render->DrawTexture(coins, -app->render->camera.x + (i * 17)+129, 0, NULL);
	}

	SDL_Rect floppyRect = currentFloppy->GetCurrentFrame();

	if (fCount < 180)
	{
		app->render->DrawTexture(floppyDisk, -app->render->camera.x, 40, &floppyRect);
		fCount++;
	}

	return ret;
}

bool Player::CleanUp()
{
	LOG("Freeing player");
	app->tex->UnLoad(character);
	app->tex->UnLoad(floppyDisk);
	app->tex->UnLoad(lives);
	app->tex->UnLoad(coins);

	app->player->active = false;
	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	loadPosition = true;
	pugi::xml_node play = data.child("position");

	cp.x = play.attribute("x").as_int(0);
	cp.y = play.attribute("y").as_int(0);

	pugi::xml_node scene = data.child("sceneValue");
	
	sceneValue = scene.attribute("l").as_int(0);


		switch (sceneValue)
		{
		case 1:
			if (sceneTwoA)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->scene, 1);
			}	
			if (sceneThreeA)
			{
				app->fade->Fade((Module*)app->scene3, (Module*)app->scene, 1);
			}	
			break;
		case 2:
			if (sceneOneA)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 1);
			}
			if (sceneThreeA)
			{
				app->fade->Fade((Module*)app->scene3, (Module*)app->scene2, 1);
			}
			break;
		case 3:
			if (sceneOneA)
			{
				app->fade->Fade((Module*)app->scene, (Module*)app->scene3, 1);
			}
			if (sceneTwoA)
			{
				app->fade->Fade((Module*)app->scene2, (Module*)app->scene3, 1);
			}
			break;
		}

	app->player->savedPos.x = cp.x;
	app->player->savedPos.y = cp.y;

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cp.x);
	play.attribute("y").set_value(cp.y);


	pugi::xml_node scene = data.child("sceneValue");
	
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
	scene.attribute("l").set_value(sceneValue);

	app->player->savedPos.x = cp.x;
	app->player->savedPos.y = cp.y;

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (!godMode)
	{
		switch (c2->type)
		{
		case Collider::Type::COLL:
			switch (c1->type)
			{
			case Collider::Type::playerTop:
				cp.y = c2->rect.y + c2->rect.h;
				vcy = 0.0f;
				break;
			case Collider::Type::playerBot:
				cp.y = c2->rect.y - 110;
				vcy = 0.0f;
				ong = true;
				jumps = 0;
				break;
			case Collider::Type::playerRight:
				cp.x = c2->rect.x - 66 + 1;
				xMove = false;
				break;
			case Collider::Type::playerLeft:
				cp.x = c2->rect.x + c2->rect.w - 1;
				xMove = false;
				break;
			}
			break;
		case Collider::Type::WIN:
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene, (Module*)app->scene2, 60);
			break;
		case Collider::Type::WIN2:
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene2, (Module*)app->scene3, 60);
			break;
		case Collider::Type::WIN3:
			c2->pendingToDelete = true;
			app->fade->Fade((Module*)app->scene3, (Module*)app->titleScreen, 60);
			winScreen = true;
			break;
		case Collider::Type::DEATH:
			if (!minusLives)
			{
				c2->pendingToDelete = true;
				minusLives = true;
				--playerLives;
				if (playerLives == 0)
				{
					if (sceneOneA)
					{
						app->fade->Fade((Module*)app->scene, (Module*)app->titleScreen, 30);
					}
					if (sceneTwoA == true)
					{
						app->fade->Fade((Module*)app->scene2, (Module*)app->titleScreen, 30);
					}
					if (sceneThreeA == true)
					{
						app->fade->Fade((Module*)app->scene3, (Module*)app->titleScreen, 30);
					}
					playerLives = 3;
					loseScreen = true;
				}
				else
				{
					if (sceneTwoA)
					{
						app->fade->Fade((Module*)app->scene2, (Module*)app->scene2, 30);
					}
					if (sceneThreeA)
					{
						app->fade->Fade((Module*)app->scene3, (Module*)app->scene3, 30);
					}
				}
			}
			break;
		case Collider::Type::CHECKPOINT:
			if (ong)
			{
				app->audio->PlayFx(checkPointFx);
				c2->pendingToDelete = true;
				app->SaveGameRequest();
				app->scene->flagAlive = false;
				app->scene2->flagAlive = false;
				fCount = 0;
			}
			break;
		default:
			break;
		}
	}
}
	
void Player::resetPlayer()
{
	if (loadPosition)
	{
		cp.x = app->player->savedPos.x;
		cp.y = app->player->savedPos.y;

		loadPosition = false;
	}
	else
	{
		if (sceneOneA == true)
		{
			app->player->cp.x = 80;
			app->player->cp.y = 0;
		}
		if (sceneTwoA == true)
		{
			app->player->cp.x = 100;
			app->player->cp.y = 0;
		}
		if (sceneThreeA == true)
		{
			app->player->cp.x = 90;
			app->player->cp.y = 0;
		}

		app->player->vcy = 0;
		app->player->vcx = 200;
		app->player->jump = -350;

		app->render->camera.x = 0;
		app->render->camera.y = 0;

		app->player->xMove = false;
		app->player->ong = false;
	}
	
}