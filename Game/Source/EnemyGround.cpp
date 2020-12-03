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
#include "EnemyGround.h"

#include "Defs.h"
#include "Log.h"

EnemyGround::EnemyGround() : Module()
{
	name.Create("enemies");

	enemyIdleL.speed = 0.05f;
	enemyIdleL.loop = true;
	enemyIdleL.PushBack({ 468,593,84,93 });
	enemyIdleL.PushBack({ 469,492,83,93 });
	enemyIdleL.PushBack({ 470,394,79,94 });
	enemyIdleL.PushBack({ 468,300,80,94 });
	
}

EnemyGround::~EnemyGround()
{

}

bool EnemyGround::Start()
{
	ep.x = 205;//70
	ep.y = 180;//500

	groundEnemy = app->tex->Load("Assets/Characters/first_enemy.png");

	ep.w = 80;
	ep.h = 95;

	enemyGroundCol = app->collisions->AddCollider(ep, Collider::Type::ENEMY, (Module*)app->player);

	facingLeft = true;
	facingRight = false;

	enemyGroundCurrentAnimation = &enemyIdleL;

	return true;
}

bool EnemyGround::Awake()
{
	bool ret = true;

	return ret;
}

bool EnemyGround::PreUpdate()
{
	return true;
}

bool EnemyGround::Update(float dt)
{
	enemyGroundCurrentAnimation = &enemyIdleL;
	enemyGroundCurrentAnimation->Update();
	enemyGroundCol->SetPos(ep.x, ep.y);

	return true;
}

bool EnemyGround::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectEnemy;
	rectEnemy = enemyGroundCurrentAnimation->GetCurrentFrame();

	app->render->DrawTexture(groundEnemy, ep.x, ep.y, &rectEnemy); // Placeholder not needed any more
	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool EnemyGround::CleanUp()
{
	app->enemies->active = false;
	return true;
}
bool EnemyGround::LoadState(pugi::xml_node& data)
{
	/*pugi::xml_node play = data.child("position");
	cp.x = play.attribute("x").as_int(0);
	cp.y = play.attribute("y").as_int(0);
	*/
	return true;
}

bool EnemyGround::SaveState(pugi::xml_node& data) const
{
	/*pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(cp.x);
	play.attribute("y").set_value(cp.y);*/

	return true;
}

void EnemyGround::OnCollision(Collider* c1, Collider* c2)
{

}