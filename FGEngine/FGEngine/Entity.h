#pragma once
#include <vector>

#include "Vector2D.h"
#include "CircleCollider.h"
#include <iostream>

struct SDL_Renderer;

namespace FG
{
	class Camera;
	class Entity
	{
	public:
		Entity() {}
		virtual ~Entity() {	}

		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera) {}
		virtual Vector2D& GetPosition() = 0;
		bool CheckCollision(Entity& other)
		{
			if ((GetPosition() - other.GetPosition()).Magnitude() < collider.radius + other.collider.radius)
			{
				std::cout << "coll" << std::endl;
				return true;
			}
			return false;
		}
		CircleCollider collider;
	};
}
