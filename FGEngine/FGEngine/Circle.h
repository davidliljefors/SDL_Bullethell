#pragma once
#include "Vector2D.h"

namespace FG
{
	struct Circle
	{

		Circle(const Vector2D* const vec, float r)
		{
			position = vec;
			if (r < 0.f)
				r = 0.f;
			radius = r;
		}
		Circle(const Vector2D* const vec, float offsetX, float offsetY, float r)
		{
			offset.x = offsetX;
			offset.y = offsetY;
			position = vec;
			if (r < 0.f)
				r = 0.f;
			radius = r;
		}

		float GetRadius() const { return radius; }
		void SetRadius(float size)  { radius = size; }
		Vector2D GetPosition() const { return *position + offset; }

	private:
		float radius = 0.0f;
		Vector2D offset = { 0.f, 0.f };
		const Vector2D* position;
		Circle();
	};
}
