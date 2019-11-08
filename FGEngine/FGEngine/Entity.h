#pragma once
#include <vector>
#include <iostream>

#include "Vector2D.h"
#include "CircleCollider.h"
#include "EntityManager.h"


struct SDL_Renderer;
struct SDL_Texture;

namespace FG
{
	static unsigned int entityCounter = 0;
	unsigned static int GetNewEntityID()
	{
		return entityCounter++;
	}

	struct Sprite;
	class Camera;
	class Entity
	{


	public:
		Entity() {}
		Entity(EntityManager* manager) : manager(manager) {}
		virtual ~Entity() {	}
		virtual void Update(float deltaTime) {}
		virtual void Render(Camera* const camera);
		virtual void Collided(Entity& other) {}
		virtual void Shoot() {}
		/// Marks object for deletion at end of frame
		virtual void Destroy() { markedForDestroy = true; }

		inline bool CheckCollision(Entity& other)
		{	if ((GetPosition() - other.GetPosition()).Magnitude() < collider.radius + other.collider.radius)
			{return true;}
			return false;}

		inline unsigned int GetID()		{ return entityId; }
		inline Vector2D& GetPosition()	{ return position; }

		CircleCollider collider;
		void SetSprite(Sprite* sprite) { this->sprite = sprite; }
	protected:
		unsigned int entityId = GetNewEntityID();
		EntityManager* manager = nullptr;
		Sprite* sprite = nullptr;
		Vector2D position = { 0,0 };

	private:
		// Entity manager needs access to marked for destroy
		friend class EntityManager;
		bool markedForDestroy = false;
	};


}
