#include <algorithm>
#include "EntityManager.h"
#include "Entity.h"
#include "Collision.h"

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
		if (addList.size() > 0)
		{
			for (auto entity : addList)
			{
				entities.push_back(entity);
			}
			addList.clear();
		}

		for (auto entity : entities)
		{
			entity->Update(deltaTime);
		}

		CleanDestroyedObjects();
	}

	void EntityManager::Render(Camera* const camera)
	{
		for (auto entity : entities)
		{
			entity->Render(camera);
		}
	}

	void EntityManager::DoCollisions()
	{
		for (int x = 0; x < entities.size() - 1; x++)
		{
			for (int y = x + 1; y < entities.size(); y++)
			{
				if (entities[x] && entities[y])
				{
					if (entities[x]->Dead() || entities[y]->Dead())
					{
						continue;
					}
					if (entities[x]->IgnoreCollision() || entities[y]->IgnoreCollision())
						continue;
					if (!entities[x]->GetColliderCircle() || !entities[y]->GetColliderCircle())
						continue;

					if ((entities[x]->collisionLayer & entities[y]->collisionLayer).any() &&
						Collision::CircleIntersects(*entities[x]->GetColliderCircle(), *entities[y]->GetColliderCircle()))
					{
						entities[x]->OnCollision(entities[y]);
						entities[y]->OnCollision(entities[x]);
					}
				}
			}
		}
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		addList.push_back(entity);
	}

	void EntityManager::CleanDestroyedObjects()
	{
		auto newEnd = std::remove_if(entities.begin(), entities.end(),
			[](const Entity* const e) { return e->markedForDestroy; });

		for (auto& it = newEnd; it != entities.end(); it++)
		{
			delete* it;
		}

		entities.erase(newEnd, entities.end());
	}
}
