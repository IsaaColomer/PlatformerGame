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
#include "Title.h"
#include "Enemies.h"
#include "Collectible.h"

#include "Defs.h"
#include "Log.h"

Collectible::Collectible() : Module()
{
	name.Create("collectible");
}

Collectible::~Collectible()
{

}

bool Collectible::Start()
{
	food = app->tex->Load("Assets/Screens/Gameplay/food.png");
	coin = app->tex->Load("Assets/Screens/Gameplay/coin.png");

	if (app->scene->active == true)
	{
		collectibleRect[0] = { 100,100,50,54 };
		collectibleTypes[0] = Collider::Type::FOOD;

		collectibleRect[1] = { 200,200,50,54 };
		collectibleTypes[1] = Collider::Type::FOOD;

		collectibleRect[2] = { 300,300,50,54 };
		collectibleTypes[2] = Collider::Type::FOOD;

		collectibleRect[3] = { 400,400,50,54 };
		collectibleTypes[3] = Collider::Type::COIN;

		collectibleRect[4] = { 500,500,50,54 };
		collectibleTypes[4] = Collider::Type::COIN;

		for (int i = 0; i < MAX_COLLECTIBLES; i++)
			collectibleCol[i] = app->collisions->AddCollider(collectibleRect[i], collectibleTypes[i], this);
	}

	else if (app->scene2->active == true)
	{
		collectibleRect[0] = { 100,100,50,54 };
		collectibleTypes[0] = Collider::Type::FOOD;

		collectibleRect[1] = { 200,100,50,54 };
		collectibleTypes[1] = Collider::Type::FOOD;

		collectibleRect[2] = { 300,100,50,54 };
		collectibleTypes[2] = Collider::Type::FOOD;

		collectibleRect[3] = { 400,100,50,54 };
		collectibleTypes[3] = Collider::Type::COIN;

		collectibleRect[4] = { 500,100,50,54 };
		collectibleTypes[4] = Collider::Type::COIN;

		for (int i = 0; i < MAX_COLLECTIBLES; i++)
			collectibleCol[i] = app->collisions->AddCollider(collectibleRect[i], collectibleTypes[i], this);
	}

	return true;
}

bool Collectible::Awake()
{
	bool ret = true;

	return ret;
}

bool Collectible::PreUpdate()
{
	return true;
}

bool Collectible::Update(float dt)
{
	for (int i = 0; i < MAX_COLLECTIBLES; i++)
	{
		if (collectibleCol[i] != nullptr)
		{
			if (collectibleTypes[i] == Collider::Type::FOOD)
				app->render->DrawTexture(food, collectibleRect[i].x, collectibleRect[i].y, NULL);
			else if (collectibleTypes[i] == Collider::Type::COIN)
				app->render->DrawTexture(coin, collectibleRect[i].x, collectibleRect[i].y, NULL);
		}
	}

	return true;
}

bool Collectible::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Collectible::CleanUp()
{
	app->tex->UnLoad(food);
	app->tex->UnLoad(coin);

	/*for (auto i : collectibleCol)
		i->pendingToDelete = true;*/

	return true;
}

void Collectible::OnCollision(Collider* c1, Collider* c2)
{

}