#pragma once
#include <cassert>
#include <bitset>
#include <SDL_rect.h>
#include "Sprite.h"
#include "Circle.h"


namespace FG
{
	class Camera;

	class Entity

	{
		friend class EntityManager;
	public:
		
		virtual ~Entity() {}
		virtual void Update(float deltaTime) {}
		virtual SDL_Rect GetColliderRect() = 0;
		virtual void Render(Camera* const camera);
		virtual bool IgnoreCollision();
		void Destroy() { markedForDestroy = true; }
		bool Dead() const { return dead; }

		void AddCircleCollider(float radius);
		void AddCircleCollider(float offsetX, float offsetY, float radius);

		Circle GetColliderCircle() const
		{
			assert(collider);
			return *collider;
		}
		virtual void OnCollision(Entity* other) {}

		Sprite* sprite = nullptr;
		Circle* collider = nullptr;
		Vector2D position;
	protected:

		bool dead;

		std::bitset<8> collisionLayer;
	private:
		bool markedForDestroy = false;
	};
}
