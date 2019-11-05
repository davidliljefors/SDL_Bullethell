#include "TextureManager.h"
#include <cassert>
namespace FG
{
	SDL_Texture* TextureManager::LoadTexture(const char* fileName)
	{
		assert(renderer != nullptr);
		SDL_Surface* tempSurface = IMG_Load(fileName);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		return tex;
	}

	void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
	{
		assert(renderer != nullptr);
		SDL_RenderCopyEx(renderer, tex, &src, &dest, NULL, NULL, flip);
	}
}

