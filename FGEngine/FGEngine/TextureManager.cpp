#include "TextureManager.h"
#include <algorithm>
#include <iostream>

namespace FG
{

	SDL_Renderer* TextureManager::renderer;
	std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;

	void TextureManager::Initialize(SDL_Renderer* rend)
	{
		renderer = rend;
	}

	SDL_Texture* TextureManager::GetTexture(const std::string& filename)
	{
		if (textureCache.find(filename) != textureCache.end())
		{
			return textureCache[filename];
		}
		SDL_Surface* tempSurface = SDL_LoadBMP(filename.c_str());
		std::cout << SDL_GetError();
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
		textureCache[filename] = tex;
		SDL_FreeSurface(tempSurface);
		return tex;
	}
}

