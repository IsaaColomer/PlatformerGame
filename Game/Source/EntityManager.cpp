#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "List.h"
#include "Textures.h"
#include "Food.h"
#include "Coin.h"
#include "Entity.h"
#include "EnemyGround.h"
#include "EnemyAir.h"

//#include "GroundEnemy.h"
//#include "Hearts.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

bool EntityManager::Awake()
{
	return true;
}

bool EntityManager::Start()
{
	// Start for ground enemy
	SDL_Rect collider = { 0,0,80,95 };
	coinTexture = app->tex->Load("Assets/Screens/Gameplay/coin.png");
	foodTexture = app->tex->Load("Assets/Screens/Gameplay/food.png");
	enemyTexture = app->tex->Load("Assets/Characters/first_enemy.png");
	enemyAirTexture = app->tex->Load("Assets/Characters/second_enemy.png");

	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	ListItem<Entity*>* entity = entityList.start;

	while (entity != nullptr)
	{
		if (entity->data->pendingToDelete)
		{
			delete entity->data;
			entityList.del(entity);
			entity = entity->next;
			continue;
		}

		entity->data->Update(dt);
		entity = entity->next;
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}

	return true;
}

bool EntityManager::CleanUp()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->pendingToDelete = true;
	}
	return true;
}

void EntityManager::AddEntity(fPoint position, Entity::Type type)
{
	switch (type)
	{
	case Entity::Type::ENEMYG:
		enemyG = (Entity*)(new EnemyGround((Module*)this, position, enemyTexture, type));
		entityList.add(enemyG);
		break;
	case Entity::Type::COIN:
		coin = (Entity*)(new Coin((Module*)this, position, coinTexture, type));
		entityList.add(coin);
		break;
	case Entity::Type::FOOD:
		food = (Entity*)(new Food((Module*)this, position, foodTexture, type));
		entityList.add(food);
		break;
	case Entity::Type::ENEMYA:
		enemyA = (Entity*)(new EnemyAir((Module*)this, position, enemyAirTexture, type));
		entityList.add(enemyA);
		break;
	}
}

void EntityManager::DeleteEntity()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->pendingToDelete = true;

	}
}

void EntityManager::OnCollision(Collider* a, Collider* b)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);

		if (entity->data->collider == a && b != nullptr)
		{
			entity->data->Collision(b);
		}

		if (entity->data->collider == b && a != nullptr)
		{
			entity->data->Collision(a);
		}
	}
}