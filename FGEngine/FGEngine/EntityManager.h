#pragma once

#include <vector>

namespace FG
{
	class Entity;
	class Camera;
	class EntityManager
	{
	public:
		void Shutdown();
		void Update(float deltaTime);
		void Render(Camera* const camera);
		void DoCollisions();

		void AddEntity(Entity* entity);
		template<typename T, typename... TArgs>
		T* AddEntity(TArgs&&... args)
		{
			T* entity(new T(std::forward<TArgs>(args)...));
			assert(entity);
			addList.push_back(entity);
			return entity;
		}
		void CleanDestroyedObjects();

	private:
		std::vector<Entity*> entities;
		std::vector<Entity*> addList;
	};
}