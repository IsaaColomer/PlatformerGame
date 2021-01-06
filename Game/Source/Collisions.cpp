#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Log.h"
#include "Render.h"

Collisions::Collisions(bool startEnabled) : Module()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::playerLeft][Collider::Type::playerLeft] = false;
	matrix[Collider::Type::playerLeft][Collider::Type::playerRight] = false;
	matrix[Collider::Type::playerLeft][Collider::Type::COLL] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::playerTop] = false;
	matrix[Collider::Type::playerLeft][Collider::Type::WIN] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::WIN2] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::DEATH] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::playerBot] = false;
	matrix[Collider::Type::playerLeft][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::FOOD] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::CHECKPOINT] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::COIN] = true;
	matrix[Collider::Type::playerLeft][Collider::Type::WIN3] = true;
	
	matrix[Collider::Type::playerRight][Collider::Type::playerLeft] = false;
	matrix[Collider::Type::playerRight][Collider::Type::playerRight] = false;
	matrix[Collider::Type::playerRight][Collider::Type::COLL] = true;
	matrix[Collider::Type::playerRight][Collider::Type::playerTop] = false;
	matrix[Collider::Type::playerRight][Collider::Type::WIN] = true;
	matrix[Collider::Type::playerRight][Collider::Type::DEATH] = true;
	matrix[Collider::Type::playerRight][Collider::Type::playerBot] = false;
	matrix[Collider::Type::playerRight][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::playerRight][Collider::Type::WIN2] = true;
	matrix[Collider::Type::playerRight][Collider::Type::FOOD] = true;
	matrix[Collider::Type::playerRight][Collider::Type::CHECKPOINT] = true;
	matrix[Collider::Type::playerRight][Collider::Type::COIN] = true;
	matrix[Collider::Type::playerRight][Collider::Type::WIN3] = true;

	matrix[Collider::Type::COLL][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::COLL][Collider::Type::playerRight] = true;
	matrix[Collider::Type::COLL][Collider::Type::COLL] = false;
	matrix[Collider::Type::COLL][Collider::Type::playerTop] = true;
	matrix[Collider::Type::COLL][Collider::Type::playerBot] = true;
	matrix[Collider::Type::COLL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::COLL][Collider::Type::WIN] = false;
	matrix[Collider::Type::COLL][Collider::Type::DEATH] = false;
	matrix[Collider::Type::COLL][Collider::Type::WIN2] = false;
	matrix[Collider::Type::COLL][Collider::Type::FOOD] = false;
	matrix[Collider::Type::COLL][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::COLL][Collider::Type::COIN] = false;

	matrix[Collider::Type::playerTop][Collider::Type::playerLeft] = false;
	matrix[Collider::Type::playerTop][Collider::Type::playerRight] = false;
	matrix[Collider::Type::playerTop][Collider::Type::COLL] = true;
	matrix[Collider::Type::playerTop][Collider::Type::playerTop] = false;
	matrix[Collider::Type::playerTop][Collider::Type::playerBot] = false;
	matrix[Collider::Type::playerTop][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::playerTop][Collider::Type::WIN] = true;
	matrix[Collider::Type::playerTop][Collider::Type::DEATH] = true;
	matrix[Collider::Type::playerTop][Collider::Type::WIN2] = true;
	matrix[Collider::Type::playerTop][Collider::Type::FOOD] = true;
	matrix[Collider::Type::playerTop][Collider::Type::CHECKPOINT] = true;
	matrix[Collider::Type::playerTop][Collider::Type::COIN] = true;
	matrix[Collider::Type::playerTop][Collider::Type::WIN3] = true;

	matrix[Collider::Type::playerBot][Collider::Type::playerLeft] = false;
	matrix[Collider::Type::playerBot][Collider::Type::playerRight] = false;
	matrix[Collider::Type::playerBot][Collider::Type::COLL] = true;
	matrix[Collider::Type::playerBot][Collider::Type::playerTop] = false;
	matrix[Collider::Type::playerBot][Collider::Type::playerBot] = false;
	matrix[Collider::Type::playerBot][Collider::Type::WIN] = true;
	matrix[Collider::Type::playerBot][Collider::Type::DEATH] = true;
	matrix[Collider::Type::playerBot][Collider::Type::WIN2] = true;
	matrix[Collider::Type::playerBot][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::playerBot][Collider::Type::FOOD] = true;
	matrix[Collider::Type::playerBot][Collider::Type::CHECKPOINT] = true;
	matrix[Collider::Type::playerBot][Collider::Type::COIN] = true;
	matrix[Collider::Type::playerBot][Collider::Type::WIN3] = true;
	matrix[Collider::Type::playerBot][Collider::Type::PLAYERBOT] = true;

	matrix[Collider::Type::WIN][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::WIN][Collider::Type::playerRight] = true;
	matrix[Collider::Type::WIN][Collider::Type::COLL] = false;
	matrix[Collider::Type::WIN][Collider::Type::playerTop] = true;
	matrix[Collider::Type::WIN][Collider::Type::playerBot] = true;
	matrix[Collider::Type::WIN][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN][Collider::Type::COIN] = false;

	matrix[Collider::Type::DEATH][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::DEATH][Collider::Type::playerRight] = true;
	matrix[Collider::Type::DEATH][Collider::Type::COLL] = false;
	matrix[Collider::Type::DEATH][Collider::Type::playerTop] = true;
	matrix[Collider::Type::DEATH][Collider::Type::playerBot] = true;
	matrix[Collider::Type::DEATH][Collider::Type::WIN] = false;
	matrix[Collider::Type::DEATH][Collider::Type::DEATH] = false;
	matrix[Collider::Type::DEATH][Collider::Type::WIN2] = false;
	matrix[Collider::Type::DEATH][Collider::Type::FOOD] = false;
	matrix[Collider::Type::DEATH][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::DEATH][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::DEATH][Collider::Type::COIN] = false;

	matrix[Collider::Type::WIN2][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::WIN2][Collider::Type::playerRight] = true;
	matrix[Collider::Type::WIN2][Collider::Type::COLL] = false;
	matrix[Collider::Type::WIN2][Collider::Type::playerTop] = true;
	matrix[Collider::Type::WIN2][Collider::Type::playerBot] = true;
	matrix[Collider::Type::WIN2][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN2][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN2][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN2][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN2][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN2][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN2][Collider::Type::COIN] = false;

	matrix[Collider::Type::WIN3][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::WIN3][Collider::Type::playerRight] = true;
	matrix[Collider::Type::WIN3][Collider::Type::COLL] = false;
	matrix[Collider::Type::WIN3][Collider::Type::playerTop] = true;
	matrix[Collider::Type::WIN3][Collider::Type::playerBot] = true;
	matrix[Collider::Type::WIN3][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN3][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN3][Collider::Type::WIN2] = false;
	matrix[Collider::Type::WIN3][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN3][Collider::Type::FOOD] = false;
	matrix[Collider::Type::WIN3][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::WIN3][Collider::Type::COIN] = false;

	matrix[Collider::Type::ENEMY][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::playerRight] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::COLL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::playerTop] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::playerBot] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::DEATH] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::WIN] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::WIN2] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::FOOD] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYERBOT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::COIN] = false;

	matrix[Collider::Type::PLAYERBOT][Collider::Type::playerLeft] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::playerRight] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::COLL] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::playerTop] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::playerBot] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::DEATH] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::WIN] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::WIN2] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::FOOD] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::PLAYERBOT][Collider::Type::COIN] = false;

	matrix[Collider::Type::FOOD][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::FOOD][Collider::Type::playerRight] = true;
	matrix[Collider::Type::FOOD][Collider::Type::COLL] = false;
	matrix[Collider::Type::FOOD][Collider::Type::playerTop] = true;
	matrix[Collider::Type::FOOD][Collider::Type::playerBot] = true;
	matrix[Collider::Type::FOOD][Collider::Type::DEATH] = false;
	matrix[Collider::Type::FOOD][Collider::Type::WIN] = false;
	matrix[Collider::Type::FOOD][Collider::Type::WIN2] = false;
	matrix[Collider::Type::FOOD][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::FOOD][Collider::Type::FOOD] = false;
	matrix[Collider::Type::FOOD][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::FOOD][Collider::Type::COIN] = false;

	matrix[Collider::Type::COIN][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::COIN][Collider::Type::playerRight] = true;
	matrix[Collider::Type::COIN][Collider::Type::COLL] = false;
	matrix[Collider::Type::COIN][Collider::Type::playerTop] = true;
	matrix[Collider::Type::COIN][Collider::Type::playerBot] = true;
	matrix[Collider::Type::COIN][Collider::Type::DEATH] = false;
	matrix[Collider::Type::COIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::COIN][Collider::Type::WIN2] = false;
	matrix[Collider::Type::COIN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::COIN][Collider::Type::FOOD] = false;
	matrix[Collider::Type::COIN][Collider::Type::CHECKPOINT] = false;
	matrix[Collider::Type::COIN][Collider::Type::COIN] = false;

	matrix[Collider::Type::CHECKPOINT][Collider::Type::playerLeft] = true;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::playerRight] = true;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::COLL] = false;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::playerTop] = true;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::playerBot] = true;
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

		case Collider::Type::COLL:
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::playerLeft:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case Collider::Type::playerRight:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case Collider::Type::playerTop:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case Collider::Type::playerBot:
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 255, alpha);
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
		case Collider::Type::PLAYERBOT:
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		}

	}
}

void Collisions::DebugRequest()
{
	debug = !debug;
}