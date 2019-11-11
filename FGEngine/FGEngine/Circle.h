#pragma once
#include "Vector2D.h"

namespace FG
{
	struct Circle
	{
		Circle(float x, float y, float r)
		{
			position.x = x;
			position.y = y;
			radius = r;
		}
		Circle(const Vector2D& vec, float r)
		{
			position.x = vec.x;
			position.y = vec.y;
			radius = r;
		}
		Vector2D position = { 0.0f, 0.0f };
		float radius = 0.0f;
	};
}
