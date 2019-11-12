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
		for (auto entity : entities)
		{
			entity->Update(deltaTime);
		}
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
				if (Collision::CircleIntersects(entities[x]->GetColliderCircle(), entities[y]->GetColliderCircle()))
				{
					entities[x]->OnCollision(entities[y]);
					entities[y]->OnCollision(entities[x]);
				}
				if (Collision::AABB(entities[x]->GetColliderRect(), entities[y]->GetColliderRect()))
				{
					
				}
			}
		}
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	}
}
