#pragma once
#include <SDL_rect.h>

namespace FG
{
	class Camera;
	class Entity
	{
	public:
		virtual ~Entity() {}

		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera) {}
		virtual SDL_Rect GetColliderRect() = 0;
		virtual void OnCollision(Entity* other) {}
	};
}
