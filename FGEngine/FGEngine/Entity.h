#pragma once
#include <cassert>
#include <SDL_rect.h>
#include "Sprite.h"
#include "Circle.h"
namespace FG
{
	class Camera;
	class Entity

	{
	public:
		virtual ~Entity() {}

		virtual void Update(float deltaTime) {}
		virtual SDL_Rect GetColliderRect() = 0;
		virtual void Render(Camera* const camera);

		void AddCircleCollider(float radius)
		{
			collider = new Circle(&position, radius);
		}
		void AddCircleCollider(float offsetX, float offsetY, float radius)
		{
			collider = new Circle(&position, offsetX, offsetY, radius);
		}
		Circle GetColliderCircle() const
		{
			assert(collider);
			return *collider;
		}
		virtual void OnCollision(Entity* other) {}

		Sprite* sprite = nullptr;
		Circle* collider = nullptr;
		Vector2D position;
	};
}
