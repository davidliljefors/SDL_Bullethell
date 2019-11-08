#pragma once
#include <SDL.h>
#include "Vector2D.h"
#include "Camera.h"
#include "Entity.h"

namespace FG
{
	struct Sprite
	{
		Sprite(Entity* parent, SDL_Texture* tex, int srcW, int srcH, int w, int h);
		
		Entity* parent = nullptr;
		SDL_Rect src;
		SDL_Rect dst;
		SDL_Texture* tex;
	};
}