#include "EntityManager.h"
#include "Entity.h"

#include <assert.h>

namespace FG
{
	void EntityManager::Shutdown()
	{
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			delete* it;
		}
		entities.clear();
	}

	void EntityManager::Update(float deltaTime)
	{
		for (auto& entity : entities)
		{
			entity->Update(deltaTime);
		}
		CheckCollisions();
	}

	void EntityManager::Render(Camera* const camera)
	{
		for (auto& entity : entities)
		{
			entity->Render(camera);
		}
	}

	void EntityManager::CheckCollisions()
	{
		if (entities.size() > 1)
		{
			for (size_t i = 1; i < entities.size(); i++)
			{
				entities[i]->CheckCollision(*entities[i - 1LL]);
			}

		}
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		assert(entity);
		entities.push_back(entity);
	}
}
