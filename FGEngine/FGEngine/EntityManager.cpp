#include "EntityManager.h"
#include "Entity.h"

#include <assert.h>

namespace FG
{
	void EntityManager::Shutdown()
	{
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			delete *it;
		}
		entities.clear();
	}

	void EntityManager::Update(float deltaTime)
	{
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->Update(deltaTime);
		}
	}

	void EntityManager::Render(Camera* const camera)
	{
		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			(*it)->Render(camera);
		}
	}

	void EntityManager::AddEntity(Entity* entity)
	{
		assert(entity);
		entities.push_back(entity);
	}
}
