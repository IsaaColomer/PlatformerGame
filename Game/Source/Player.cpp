#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
//#include "Collisions.h"


#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	idleAnimR.loop = true;
	idleAnimR.PushBack({ 0,593,84,93 });
	idleAnimR.PushBack({ 0,492,83,93 });
	idleAnimR.PushBack({ 3,394,79,94 });
	idleAnimR.PushBack({ 4,300,80,94 });
	
	idleAnimR.speed = 0.05f;


	idleAnimL.loop = true;
	idleAnimL.PushBack({ 468,593,84,93 });
	idleAnimL.PushBack({ 469,492,83,93 });
	idleAnimL.PushBack({ 470,394,79,94 });
	idleAnimL.PushBack({ 468,300,80,94 });

	idleAnimL.speed = 0.05f;
	//idleAnim.PushBack({ 43,87,20,37 });

	jumpAnim.loop = false;
	
	jumpAnim.PushBack({ 133,292,86,95 });
	jumpAnim.PushBack({ 127,593,73,93 });
	jumpAnim.PushBack({ 121,492,85,93 });
	jumpAnim.PushBack({ 143,393,66,95 });
	jumpAnim.speed = 0.05f;


	rightAnim.PushBack({243,592,75,94});
	rightAnim.PushBack({ 249,492,69,93 });
	rightAnim.PushBack({ 249,394,77,94 });
	rightAnim.PushBack({ 243,292,75,94 });
	rightAnim.PushBack({ 248,187,70,94 });
	rightAnim.PushBack({ 251,80,67,94 });
	rightAnim.loop = true;
	rightAnim.speed = 0.1f;
	//HERE ALL THE ANIMATIONS

	leftAnim.PushBack({358,592,75,94});
	leftAnim.PushBack({358,492,69,93});
	leftAnim.PushBack({350,394,77,94});
	leftAnim.PushBack({358,292,75,94});
	leftAnim.PushBack({358,187,70,94});
	leftAnim.PushBack({358,80,67,94});
	leftAnim.loop = true;
	leftAnim.speed = 0.05f;


}

Player::~Player()
{

}

bool Player::Start()
{
	cpx = 70;
	cpy = 590;
	vcy = 0;
	vcx = 2.0f;
	facingLeft = false;
	facingRight = true;
	//ANIMATION FILE
	character = app->tex->Load("Assets/player/idle.png");
	currentAnimation = &idleAnimR;
	ong = false;

	//PLAYER RECT
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
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& facingLeft == false) {
		currentAnimation = &idleAnimR;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& facingLeft == true) {
		currentAnimation = &idleAnimL;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = (godMode) ? false : true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		cpx = 70;
		cpy = 590;
		vcy = 0;
		ong = false;
		app->render->camera.y = 0;
		app->render->camera.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		cpx = savedx;
		cpy = savedy;

	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
		savedx = cpx;
		savedy = cpy;
	}
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debugDraw = (debugDraw) ? false : true;
	}
	//player movement
	if (!godMode)
	{
		ong = false;
		for (int i = 0; i < 11 && !ong; i++)
		{
			if (((cpy + 85) > (coll[i][0] - 3)) && ((cpy + 85) < (coll[i][0] + 3)))
			{
				if (cpx<coll[i][1] && (cpx + 80)>coll[i][1])
					ong = true;
				else if (cpx<coll[i][2] && (cpx + 80)>coll[i][2])
					ong = true;
				else if (cpx > coll[i][1] && (cpx + 80) < coll[i][2])
					ong = true;
				else if (cpx<coll[i][1] && (cpx + 80)>coll[i][2])
					ong = true;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && ong)
		{
			vcy = -6.0f;
			ong = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && ong == false)
		{
			if (currentAnimation != &jumpAnim)
			{
				jumpAnim.Reset();
				currentAnimation = &jumpAnim;
			}
		}
		if (!ong)
		{
			if (vcy < 6.0f) vcy -= grav;
			cpy += vcy;
			
		}
		//if (ong)
		else vcy = 0;
	}

	else
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			cpy -= vcx;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			cpy += vcx;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cpx -= vcx;
		if (currentAnimation != &leftAnim) {
			leftAnim.Reset();
			currentAnimation = &leftAnim;
		}
		if (cpx > 640 && cpx < 1920)
		{
			app->render->camera.x += vcx;
		}
		facingLeft = true;
		facingRight = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cpx += vcx;
		if (currentAnimation != &rightAnim) {
			rightAnim.Reset();
			currentAnimation = &rightAnim;
		}
		if (cpx > 640 && cpx <= 1920)
		{
			app->render->camera.x -= vcx;
		}
		facingRight = true;
		facingLeft = false;
	}

	if (debugDraw)
	{
		SDL_Rect r;
		for (int i = 0; i < 11; i++)
		{
			r.x = coll[i][1];
			r.y = coll[i][0] - 2;
			r.w = coll[i][2] - coll[i][1];
			r.h = 4;

			app->render->DrawRectangle(r, 255, 0, 0, 128, true, true);
		}
		r.x = cpx;
		r.y = cpy;
		r.w = 80;
		r.h = 85;
		app->render->DrawRectangle(r, 255, 0, 0, 128, true, true);
	}
	currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(character, cpx, cpy, &rectPlayer); // Placeholder not needed any more

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
	cpx = play.attribute("x").as_int(0);
	cpy = play.attribute("y").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cpx);
	play.attribute("y").set_value(cpy);

	return true;
}
void Player::OnCollision(Collider* c1, Collider* c2)
{
	
}
