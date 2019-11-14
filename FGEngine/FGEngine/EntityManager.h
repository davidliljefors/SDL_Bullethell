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
		void AddEntities(Entity** array, int length);

	private:
		std::vector<Entity*> entities;
		std::vector<Entity*> addList;
	};
}