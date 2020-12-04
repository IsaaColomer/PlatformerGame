#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Log.h"
#include "Render.h"

Collisions::Collisions(bool startEnabled) : Module()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::LEFT_WALL][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::ROOF] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::WIN] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::WIN2] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::DEATH] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::FOOD] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::LEFT_WALL][Collider::Type::COIN] = false;
	
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::ROOF] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::WIN] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::DEATH] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::WIN2] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::FOOD] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::RIGHT_WALL][Collider::Type::COIN] = false;

	matrix[Collider::Type::FLOOR][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::ROOF] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::FLOOR][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::FLOOR][Collider::Type::WIN] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::DEATH] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::WIN2] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::FOOD] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::FLOOR][Collider::Type::COIN] = false;

	matrix[Collider::Type::ROOF][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::ROOF][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::ROOF][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::ROOF][Collider::Type::ROOF] = true;
	matrix[Collider::Type::ROOF][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ROOF][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::ROOF][Collider::Type::WIN] = false;
	matrix[Collider::Type::ROOF][Collider::Type::DEATH] = false;
	matrix[Collider::Type::ROOF][Collider::Type::WIN2] = false;
	matrix[Collider::Type::ROOF][Collider::Type::FOOD] = false;
	matrix[Collider::Type::ROOF][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::ROOF][Collider::Type::COIN] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::LEFT_WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::RIGHT_WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::FLOOR] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ROOF] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::DEATH] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN2] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::FOOD] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::CHECKPOINT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::COIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN3] = true;

	matrix[Collider::Type::WIN][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::WIN][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::WIN][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::WIN][Collider::Type::ROOF] = false;
	matrix[Collider::Type::WIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN][Collider::Type::COIN] = false;

	matrix[Collider::Type::DEATH][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::DEATH][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::DEATH][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::DEATH][Collider::Type::ROOF] = false;
	matrix[Collider::Type::DEATH][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::DEATH][Collider::Type::WIN] = false;
	matrix[Collider::Type::DEATH][Collider::Type::DEATH] = false;
	matrix[Collider::Type::DEATH][Collider::Type::WIN2] = false;
	matrix[Collider::Type::DEATH][Collider::Type::FOOD] = false;
	matrix[Collider::Type::DEATH][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::DEATH][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::DEATH][Collider::Type::COIN] = false;

	matrix[Collider::Type::WIN2][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::WIN2][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::WIN2][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::WIN2][Collider::Type::ROOF] = false;
	matrix[Collider::Type::WIN2][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN2][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN2][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN2][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN2][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN2][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN2][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN2][Collider::Type::COIN] = false;

	matrix[Collider::Type::WIN3][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::WIN3][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::WIN3][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::WIN3][Collider::Type::ROOF] = false;
	matrix[Collider::Type::WIN3][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN3][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN3][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN3][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN3][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN3][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN3][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN3][Collider::Type::COIN] = false;

	matrix[Collider::Type::ENEMY][Collider::Type::LEFT_WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::RIGHT_WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::FLOOR] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ROOF] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::DEATH] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::WIN] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::WIN2] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::FOOD] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::COIN] = false;

	matrix[Collider::Type::FOOD][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::FOOD][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::FOOD][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::FOOD][Collider::Type::ROOF] = false;
	matrix[Collider::Type::FOOD][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::FOOD][Collider::Type::DEATH] = false;
	matrix[Collider::Type::FOOD][Collider::Type::WIN] = false;
	matrix[Collider::Type::FOOD][Collider::Type::WIN2] = false;
	matrix[Collider::Type::FOOD][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::FOOD][Collider::Type::FOOD] = false;
	matrix[Collider::Type::FOOD][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::FOOD][Collider::Type::COIN] = false;

	matrix[Collider::Type::COIN][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::COIN][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::COIN][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::COIN][Collider::Type::ROOF] = false;
	matrix[Collider::Type::COIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::COIN][Collider::Type::DEATH] = false;
	matrix[Collider::Type::COIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::COIN][Collider::Type::WIN2] = false;
	matrix[Collider::Type::COIN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::COIN][Collider::Type::FOOD] = false;
	matrix[Collider::Type::COIN][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::COIN][Collider::Type::COIN] = false;

	matrix[Collider::Type::CHECKPOINT][Collider::Type::LEFT_WALL] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::RIGHT_WALL] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::FLOOR] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::ROOF] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::DEATH] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::WIN] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::WIN2] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::FOOD] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::COIN] = false;
}

// Called before render is available
bool Collisions::Awake(pugi::xml_node&)
{
	return true;
}
// Called before the first frame
bool Collisions::Start()
{
	return true;
}
// Called each loop iteration
bool Collisions::PreUpdate()
{
	return true;
}

bool Collisions::Update(float dt)
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

bool Collisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return true;
}

// Called before quitting
bool Collisions::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
			LOG("%d ", i);
		}
	}

	return ret;
}


void Collisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

void Collisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;

		case Collider::Type::FLOOR:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::LEFT_WALL:
			app->render->DrawRectangle(colliders[i]->rect, 255, 128, 0, alpha);
			break;
		case Collider::Type::RIGHT_WALL:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case Collider::Type::ROOF:
			app->render->DrawRectangle(colliders[i]->rect, 128, 255, 0, alpha);
			break;
		case Collider::Type::PLAYER:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::WIN:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::DEATH:
			app->render->DrawRectangle(colliders[i]->rect, 255, 100, 100, alpha);
			break;
		case Collider::Type::WIN2:
			app->render->DrawRectangle(colliders[i]->rect, 30, 100, 100, alpha);
			break;
		case Collider::Type::ENEMY:
			app->render->DrawRectangle(colliders[i]->rect, 255, 252, 100, alpha);
			break;
		case Collider::Type::FOOD:
			app->render->DrawRectangle(colliders[i]->rect, 228, 177, 0, alpha);
			break;
		case Collider::Type::CHECKPOINT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case Collider::Type::COIN:
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 0, alpha);
			break;
		case Collider::Type::WIN3:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		}

	}
}

void Collisions::DebugRequest()
{
	debug = !debug;
}