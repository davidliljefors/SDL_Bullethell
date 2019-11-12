#include "Sprite.h"
#include "Logger.h"
#include "Camera.h"


#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_stbimage.h"


namespace FG
{
	void Sprite::Dispose()
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	bool Sprite::LoadImage(SDL_Renderer* renderer, const std::string& path)
	{
		Dispose();
		SDL_Surface* surface = STBIMG_Load(path.c_str());
		if (surface)
		{
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			if (!texture)
			{
				Logger::Log(SDL_GetError(),
					Logger::RemovePathFromFile(__FILE__), __LINE__);
				SDL_FreeSurface(surface);
				return false;
			}
			else
			{
				int width = 0;
				int height = 0;
				SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
				size = { static_cast<float>(width), static_cast<float>(height) };
				SDL_FreeSurface(surface);
				return true;
			}
		}
		else
		{
			Logger::Log("Error: failed to load file " + path,
				Logger::RemovePathFromFile(__FILE__), __LINE__);
			return false;
		}


		return false;
	}

	void Sprite::Render(Camera* camera, Vector2D& position)
	{
		Vector2D finalPosition = position - camera->position;
		SDL_Rect finalRect = { static_cast<int>(finalPosition.x - (size.x / 2)), static_cast<int>(finalPosition.y - (size.y / 2)),
		static_cast<int>(size.x), static_cast<int>(size.y) };
		SDL_RenderCopy(camera->GetInternalRenderer(), texture, nullptr, &finalRect);
	}
}



