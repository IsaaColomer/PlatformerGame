#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"
#include "Food.h"

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
	gEnemyTexture = app->tex->Load("Assets/Textures/Enemy.png");
	foodTexture = app->tex->Load("Assets/Screens/Gameplay/food.png");

	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);

		if (entity->data->pendingToDelete)
		{
			entityList.del(entity);
			continue;
		}

		entity->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}

	return true;
}

bool EntityManager::CleanUp()
{
	return true;
}

void EntityManager::AddEntity(fPoint position, Entity::Type type)
{
	switch (type)
	{
	case Entity::Type::ENEMY:
	/*	gEnemy = (Entity*)(new GroundEnemy(position, gEnemyTexture, type));
		entityList.add(gEnemy);*/
		break;
	case Entity::Type::FOOD:
		food = (Entity*)(new Food(position, foodTexture, type));
		entityList.add(food);
		break;
	}
}

void EntityManager::DeleteEntity()
{
	for (int i = 0; i < entityList.count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->pendingToDelete = true;
	}
}