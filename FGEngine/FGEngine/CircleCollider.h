#pragma once
#include "Vector2D.h"
#include "Logger.h"

namespace FG
{
	class CircleCollider
	{
	public:
		CircleCollider() : radius(0) {}

		void SetSize(int radius) { Logger::Log("hekk"); this->radius = radius; }
		int radius;
	};
}