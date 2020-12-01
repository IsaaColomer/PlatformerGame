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

#include "Defs.h"
#include "Log.h"

Enemies::Enemies() : Module()
{
	name.Create("enemies");

	enemyIdleL.speed = 0.05f;
	enemyIdleL.loop = true;
	enemyIdleL.PushBack({ 468,593,84,93 });
	enemyIdleL.PushBack({ 469,492,83,93 });
	enemyIdleL.PushBack({ 470,394,79,94 });
	enemyIdleL.PushBack({ 468,300,80,94 });
	
}

Enemies::~Enemies()
{

}

bool Enemies::Start()
{
	ep.x = 205;//70
	ep.y = 180;//500

	firstEnemy = app->tex->Load("Assets/Characters/first_enemy.png");

	ep.w = 80;
	ep.h = 95;

	enemyCol = app->collisions->AddCollider(ep, Collider::Type::ENEMY, this);

	facingLeft = true;
	facingRight = false;

	
	enemyCurrentAnimation = &enemyIdleL;

	return true;
}

bool Enemies::Awake()
{
	bool ret = true;

	return ret;
}

bool Enemies::PreUpdate()
{
	return true;
}

bool Enemies::Update(float dt)
{
	enemyCurrentAnimation = &enemyIdleL;
	enemyCurrentAnimation->Update();
	enemyCol->SetPos(ep.x, ep.y);

	return true;
}

bool Enemies::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectEnemy;
	rectEnemy = enemyCurrentAnimation->GetCurrentFrame();

	app->render->DrawTexture(firstEnemy, ep.x, ep.y, &rectEnemy); // Placeholder not needed any more
	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Enemies::CleanUp()
{
	app->enemies->active = false;
	return true;
}
bool Enemies::LoadState(pugi::xml_node& data)
{
	/*pugi::xml_node play = data.child("position");
	cp.x = play.attribute("x").as_int(0);
	cp.y = play.attribute("y").as_int(0);
	*/
	return true;
}

bool Enemies::SaveState(pugi::xml_node& data) const
{
	/*pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cp.x);
	play.attribute("y").set_value(cp.y);*/

	return true;
}

void Enemies::OnCollision(Collider* c1, Collider* c2)
{

}