#include "EntityManager.h"
#include "Entity.h"
#include <iostream>
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
		auto size = entities.size();
		for (size_t i = 0; i < size; i++)
		{
			entities[i]->Update(deltaTime);
		}
		//for (auto it = entities.begin(); it != end; ++it)
		//{
		//	(*it)->Update(deltaTime);
		//}
		CheckCollisions();
	}

	void EntityManager::Render(Camera* const camera)
	{
		for (auto& entity : entities)
		{
			entity->Render(camera);
		}
	}

	bool EntityManager::CheckCollisions()
	{
		if (entities.size() > 1)
		{
			for (size_t i = 1; i < entities.size(); i++)
			{
				for (size_t j = i; j < entities.size(); j++)
				{
					//std::cout << "check col";
					if (entities[j]->CheckCollision(*entities[i - 1]))
					{
						if (entities[j]->collider.groupID != entities[i - 1]->collider.groupID)
						{
							entities[j]->Collided(*entities[i - 1]);
							entities[i - 1]->Collided(*entities[j]);
							return true;

						}
					}
				}
			}

		}
		return false;
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		assert(entity);
		entities.emplace_back(entity);
	}
	void EntityManager::DeleteEntity(Entity* entity)
	{
		std::vector<Entity*>::iterator it;
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it) == entity)
			{
				entities.erase(it);
				break;
			}
		}
	}
	void EntityManager::Clean()
	{
		std::vector<Entity*>::iterator it;
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->markedForDestroy)
			{
				entities.erase(it);
				break;
			}
		}
	}
}
