#include <algorithm>
#include "EntityManager.h"
#include "Entity.h"
#include "Collision.h"
#include "Timer.h"


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
		std::lock_guard<std::mutex> lock(muEntities);
		for (auto& entity : entities)
		{
			entity->Update(deltaTime);
		}

		for (auto& pool : pools)
		{
			auto vector = *pool;
			for (auto e : vector)
			{
				e->Update(deltaTime);
			}
		}

		if (addList.size() > 0)
		{
			for (auto entity : addList)
			{
				entities.push_back(entity);
			}
			addList.clear();
			// Sort entities on separate thread to avoid stutter
			t1 = std::thread([=] {Sort(); });
			t1.detach();
		}
	}

	void EntityManager::Render(Camera* const camera)
	{
		std::lock_guard<std::mutex> lock(muEntities);
		for (auto entity : entities)
		{
			entity->Render(camera);
		}
		for (auto& pool : pools)
		{
			auto vector = *pool;
			for (auto e : vector)
			{
				e->Render(camera);
			}
		}
	}

	void EntityManager::DoCollisions()
	{
		for (int x = 0; x < entities.size() - 1; x++)
		{
			if (entities[x]->Dead())
			{ continue; }

			for (int y = x + 1; y < entities.size(); y++)
			{
				if (entities[y]->Dead())
				{ continue; }
				if (entities[x]->IgnoreCollision() || entities[y]->IgnoreCollision())
					continue;
				if ((entities[x]->collisionLayer & entities[y]->collisionLayer).any() &&
					Collision::CircleIntersects(*entities[x]->GetColliderCircle(), *entities[y]->GetColliderCircle()))
				{
					entities[x]->OnCollision(entities[y]);
					entities[y]->OnCollision(entities[x]);
				}
			}
			for (auto pool : pools)
			{
				auto vector = *pool;
				for (auto e : vector)
				{
					if (entities[x]->IgnoreCollision())
						continue;
					if ((entities[x]->collisionLayer & e->collisionLayer).any() &&
						Collision::CircleIntersects(*entities[x]->GetColliderCircle(), *e->GetColliderCircle()))
					{
						entities[x]->OnCollision(e);
						e->OnCollision(entities[x]);
					}
				}
			}
		}


#if 0

		for (int x = 0; x < entities.size() - 1; x++)
		{
			if (!entities[x] || entities[x]->Dead())
			{ continue; }

			for (int y = x + 1; y < entities.size(); y++)
			{
				if (!entities[y] || entities[y]->Dead())
				{ continue; }

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
#endif
	}

	void EntityManager::Sort()
	{
		auto copy = entities;
		std::sort(copy.begin(), copy.end(), [](const Entity* e1, const Entity* e2)
			{
				return e1->layer < e2->layer;
			}
		);
		std::lock_guard<std::mutex> lock(muEntities);
		entities = copy;
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		addList.push_back(entity);
	}

	void EntityManager::AddPool(std::vector<Entity*>* vec)
	{
		pools.push_back(vec);
	}


	//void EntityManager::CleanInactive()
	//{
	//	
	//}

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
