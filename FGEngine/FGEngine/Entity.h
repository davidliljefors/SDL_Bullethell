#pragma once
#include <cassert>
#include <bitset>
#include <SDL_rect.h>
#include "Sprite.h"
#include "Circle.h"
#include "Animation.h"

enum class EntityLayer {
	Background,
	Character,
	Bullets
};

namespace FG
{
	class Camera;

	class Entity

	{
		friend class EntityManager;
	public:
		
		virtual ~Entity() 
		{
			if (animation)
			{
				delete animation;
			}
		}
		virtual void Update(float deltaTime);
		virtual SDL_Rect GetColliderRect() { return { 0,0,0,0 }; }
		virtual void Render(Camera* const camera);
		inline virtual bool IgnoreCollision() { return false; }
		virtual bool AddSprite(Sprite* sprite);
		void Destroy() { markedForDestroy = true; }
		bool Dead() const { return dead; }

		void AddCircleCollider(float radius);
		void AddCircleCollider(float offsetX, float offsetY, float radius);

		FG::Vector2D Lerp(FG::Vector2D& start, FG::Vector2D& end, float time);

		inline Circle* GetColliderCircle() const
		{
			return collider;
		}
		virtual void OnCollision(Entity* other) {}
		
		EntityLayer layer;

		Animation* animation = nullptr;
		Circle* collider = nullptr;
		Vector2D position;
		Sprite* sprite = nullptr;
	protected:
		std::bitset<8> collisionLayer;
		bool dead = false;

	private:
		bool markedForDestroy = false;
	};
}
