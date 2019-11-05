#pragma once

#include <SDL_image.h>
struct SDL_Renderer;

namespace FG
{
	class TextureManager
	{

	public:
		static void Init(SDL_Renderer* rend)
		{
			renderer = rend;
		}
		static SDL_Texture* LoadTexture(const char* fileName);
		static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
	private:
		static SDL_Renderer* renderer;
	};
}
