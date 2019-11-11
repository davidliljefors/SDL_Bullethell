#pragma once
#include <SDL_rect.h>
#include "Sprite.h"
#include <cassert>
namespace FG
{
	class Camera;
	class Entity

	{
	public:
		virtual ~Entity() {}

		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera)
		{
			assert(sprite);
			sprite->Render(camera, position);
		}
		virtual SDL_Rect GetColliderRect() = 0;
		virtual void OnCollision(Entity* other) {}

		FG::Sprite* sprite = nullptr;
		FG::Vector2D position;
	};
}
