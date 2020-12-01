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
	foodRect = { 670,308,50,54 };
	foodCol = app->collisions->AddCollider(foodRect, Collider::Type::FOOD, this);
	foodAlive = true;

	food = app->tex->Load("Assets/Screens/Gameplay/food.png");

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
	if (foodAlive)
	{
		app->render->DrawTexture(food, foodRect.x, foodRect.y, NULL);
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
	return true;
}

void Collectible::OnCollision(Collider* c1, Collider* c2)
{

}