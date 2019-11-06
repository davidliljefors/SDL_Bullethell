#pragma once
#include "Components.h"
#include "Vector2D.h"

namespace FG
{
	class TransformComponent : public Component
	{
	public:
		Vector2D position = { 0,0 };
		Vector2D velocity = { 0,0 };

		int height = 32;
		int width = 32;
		int scale = 1;


		TransformComponent()
		{
			position.Zero;
		}

		TransformComponent(int sc)
		{
			position.Zero;
			scale = sc;
		}

		TransformComponent(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		TransformComponent(float x, float y, int h, int w, int sc)
		{
			position.x = x;
			position.y = y;
			height = h;
			width = w;
			scale = sc;
		}

		void init() override
		{
		}
		void update() override
		{
		}

	};

}