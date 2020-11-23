#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "Collisions.h"


#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	idleAnimR.speed = 0.05f;
	idleAnimR.loop = true;
	idleAnimR.PushBack({ 0,593,84,93 });
	idleAnimR.PushBack({ 0,492,83,93 });
	idleAnimR.PushBack({ 3,394,79,94 });
	idleAnimR.PushBack({ 4,300,80,94 });

	idleAnimL.speed = 0.05f;
	idleAnimL.loop = true;
	idleAnimL.PushBack({ 468,593,84,93 });
	idleAnimL.PushBack({ 469,492,83,93 });
	idleAnimL.PushBack({ 470,394,79,94 });
	idleAnimL.PushBack({ 468,300,80,94 });

	jumpAnim.speed = 0.05f;
	jumpAnim.loop = false;
	jumpAnim.PushBack({ 133,292,86,95 });
	jumpAnim.PushBack({ 127,593,73,93 });
	jumpAnim.PushBack({ 121,492,85,93 });
	jumpAnim.PushBack({ 143,393,66,95 });

	rightAnim.speed = 0.05f;
	rightAnim.loop = true;
	rightAnim.PushBack({243,592,75,94});
	rightAnim.PushBack({ 249,492,69,93 });
	rightAnim.PushBack({ 249,394,77,94 });
	rightAnim.PushBack({ 243,292,75,94 });
	rightAnim.PushBack({ 248,187,70,94 });
	rightAnim.PushBack({ 251,80,67,94 });

	leftAnim.speed = 0.05f;
	leftAnim.loop = true;
	leftAnim.PushBack({358,592,75,94});
	leftAnim.PushBack({358,492,69,93});
	leftAnim.PushBack({350,394,77,94});
	leftAnim.PushBack({358,292,75,94});
	leftAnim.PushBack({358,187,70,94});
	leftAnim.PushBack({358,80,67,94});
}

Player::~Player()
{

}

bool Player::Start()
{
	cp.x = 70;
	cp.y = 500;
	cp.w = 50;
	cp.h = 93;
	vcy = 0;
	vcx = 2.0f;
	ong = false;

	collider = app->collisions->AddCollider(cp, Collider::Type::PLAYER, this);

	facingLeft = false;
	facingRight = true;
	//ANIMATION FILE
	character = app->tex->Load("Assets/Player/animations.png");
	currentAnimation = &idleAnimR;

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
		ong = false;
		app->render->camera.y = 0;
		app->render->camera.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		cp.x = savedx;
		cp.y = savedy;

	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
		savedx = cp.x;
		savedy = cp.y;
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
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cp.x -= vcx;
		if (currentAnimation != &leftAnim) {
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		if (cp.x > 640 && cp.x < 1920)
		{
			app->render->camera.x += 2.0f;
		}
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
		if (cp.x > 640 && cp.x <= 1920)
		{
			app->render->camera.x -= 2.0f;
		}
		facingRight = true;
		facingLeft = false;
	}
	else
	{
		ong = false;
	}

	//-----------------------COLLIDER MOVEMENT
	collider->SetPos(cp.x, cp.y);
	//--------------------------------

	currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(character, cp.x, cp.y, &rectPlayer); // Placeholder not needed any more

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Player::CleanUp()
{

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
		}
		if (c2->type == Collider::Type::RIGHT_WALL)
		{
			cp.x = c2->rect.x + c2->rect.w;
		}
		if (c2->type == Collider::Type::ROOF)
		{
			cp.y = c2->rect.y + c2->rect.h;
			vcy = 0;
		}
		if (c2->type == Collider::Type::WIN)
		{
			printf("WIN!!!");
		}
	}
}
