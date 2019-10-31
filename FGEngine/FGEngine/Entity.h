#pragma once

#include "Vector2D.h"

struct SDL_Renderer;

namespace FG
{
	class Camera;
	class Entity
	{
	public:
		Entity() {}
		virtual ~Entity() {}

		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera) {}

		Vector2D position;
	};
}
