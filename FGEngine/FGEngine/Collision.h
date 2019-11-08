#pragma once
#include <SDL_rect.h>

namespace FG
{
	class Collision
	{
	public:
		// Axis Aligned Bounding Box Collision
		static bool AABB(const SDL_Rect& a, const SDL_Rect& b);

	private:
		Collision() {}
		Collision(const Collision& other) {}
		~Collision() {}

	};

}

