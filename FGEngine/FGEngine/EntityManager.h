#pragma once

#include <vector>
#include <cassert>

class Projectile;
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
		void Sort();

		void AddEntity(Entity* entity);
		void AddPool(std::vector<Entity*>* vec);
		template<typename T, typename... TArgs>
		T* AddEntity(TArgs&&... args)
		{
			static_assert (std::is_base_of<Entity, T>::value, "AddEntity isn't type of entity");
			T* entity(new T(std::forward<TArgs>(args)...));
			assert(entity);
			addList.push_back(entity);
			return entity;
		}
		//template<typename T>
		//T* GetAndActivateEntity()
		//{
		//	assert(inactiveEntities.size() > 0);
		//	AddEntity(*inactiveEntities.begin());
		//	inactiveEntities.erase(inactiveEntities.begin());
		//	return dynamic_cast<T*>(*inactiveEntities.begin());;
		//}
		//void CleanInactive();

		
		void CleanDestroyedObjects();

	private:
		std::vector<Entity*> entities;
		std::vector<std::vector<Entity*>*> pools;
		std::vector<Entity*> addList;
	};
}