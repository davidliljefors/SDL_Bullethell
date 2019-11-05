#pragma once
#include <vector>

#include "Vector2D.h"
#include "CircleCollider.h"
#include <iostream>
#include "EntityManager.h"

struct SDL_Renderer;



namespace FG
{
	static unsigned int entityCounter = 0;
	unsigned static int GetNewEntityID()
	{
		return entityCounter++;
	}

	class Camera;
	class Entity
	{
	public:
		Entity(EntityManager& manager) : manager(manager) {}
		virtual ~Entity() {	}

		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera) {}
		virtual void Collided(Entity& other){}
		Vector2D& GetPosition()
		{
			return position;
		}

		bool CheckCollision(Entity& other)
		{
			if ((GetPosition() - other.GetPosition()).Magnitude() < collider.radius + other.collider.radius)
			{
				return true;
			}
			return false;
		}

		unsigned int GetID() { return entityId; }
		CircleCollider collider;
	protected:
		
		unsigned int entityId = GetNewEntityID();
		FG::EntityManager& manager;
		Vector2D position = { 0,0 };
	};


}
