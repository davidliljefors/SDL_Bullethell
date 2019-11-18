#pragma once

#include <SDL_rect.h>
#include "Sprite.h"

namespace FG
{
	class Sprite;

	class Animation
	{
	public:

		Animation(const Sprite* const sprite) : sprite(sprite)
		{
			rect = { 0,0,static_cast<int>(sprite->size.x),static_cast<int>(sprite->size.y) };
		}
		
		void Update(float deltaTime)
		{
			{
				rect.x = static_cast<int>((currentFrame % sprite->columns) * sprite->size.x);
				rect.y = static_cast<int>((currentFrame / sprite->columns) * sprite->size.y);

				if (timeAccumulator > timePerFrame)
				{
					timeAccumulator -= timePerFrame;
					currentFrame++;
					if (currentFrame >= sprite->columns * sprite->rows)
					{
						currentFrame = 0;
					}
				}
				timeAccumulator += deltaTime;
			}
		}

		const SDL_Rect* const GetFrame() const
		{
			return &rect;
		}

	private:
		SDL_Rect rect;
		const Sprite* sprite;
		unsigned int currentFrame = 0;
		float timePerFrame = 0.1f;
		float timeAccumulator = 0.f;

	};

}