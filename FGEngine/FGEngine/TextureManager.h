#pragma once
#include <unordered_map>
#include <string>
#include "SDL.h"


namespace FG
{
	class TextureManager
	{
	public:
		static void Initialize(SDL_Renderer* renderer);
		static SDL_Texture* GetTexture(const std::string& filename);

	private:
		static SDL_Renderer* renderer;
		static std::unordered_map<std::string, SDL_Texture*> textureCache;
	};
}