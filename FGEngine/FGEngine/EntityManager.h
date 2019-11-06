#pragma once

#include <vector>

namespace FG
{
	class Camera;
	class Entity;
	class EntityManager
	{
	public:
		void Shutdown();

		// Todo: Fix better update, delta time etc.
		void Update(float deltaTime);
		// Todo: Fix smarter rendering system.
		void Render(Camera* const camera);

		/// Todo David: make it actually good
		bool CheckCollisions();
		/// Add entity and put last in internal list
		void AddEntity(Entity* entity);
		void DeleteEntity(Entity* entity);
	private:
		std::vector<Entity*> entities;
	};
}
