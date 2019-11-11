#pragma once
#include <SDL_rect.h>
#include "Circle.h"

namespace FG
{
	class Collision
	{
	public:
		// Axis Aligned Bounding Box Collision
		static inline bool AABB(const SDL_Rect& a, const SDL_Rect& b)
		{
			return (a.x + a.w >= b.x &&
				b.x + b.w >= a.x &&
				a.y + a.h >= b.y &&
				b.y + b.h >= a.y);
		}
		static inline bool CircleIntersects(const Circle& a, const Circle& b)
		{
			return (a.position - b.position).Magnitude() < a.radius + b.radius;
		}

	private:
		Collision() {}
		Collision(const Collision& other) {}
		~Collision() {}

	};

}

