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

	idleAnim.loop = true;
	idleAnim.PushBack({ 5,0,52,96 });
	idleAnim.PushBack({ 60,0,52,99 });
	idleAnim.PushBack({ 0,96,59,115 });
	idleAnim.PushBack({ 56,107,54,103 });
	idleAnim.PushBack({ 0,213,58,100 });
	//idleAnim.PushBack({ 43,87,20,37 });


	//HERE ALL THE ANIMATIONS

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
	//ANIMATION FILE
	character = app->tex->Load("Assets/player/idle.png");
	currentAnimation = &idleAnim;

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
		if (cpx > 640 && cpx < 1920)
		{
			app->render->camera.x += vcx;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cpx += vcx;
		if (cpx > 640 && cpx <= 1920)
		{
			app->render->camera.x -= vcx;
		}
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
