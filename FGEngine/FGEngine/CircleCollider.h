#pragma once
#include "Vector2D.h"


namespace FG
{
	class CircleCollider
	{
	public:
		CircleCollider() : radius(0) {}

		void SetSize(int radius) { this->radius = radius; }
		int groupID = 0;
		int radius;
	};
}