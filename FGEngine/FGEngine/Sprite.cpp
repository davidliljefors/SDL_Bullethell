#include "Sprite.h"
#include "Logger.h"
#include "Camera.h"


#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_stbimage.h"

#include <iostream>



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

	//bool Sprite::LoadImage(SDL_Renderer* renderer, const std::string& path)
	//{
	//	Dispose();
	//	SDL_Surface* surface = STBIMG_Load(path.c_str());
	//	if (surface)
	//	{
	//		texture = SDL_CreateTextureFromSurface(renderer, surface);
	//		if (!texture)
	//		{
	//			Logger::Log(SDL_GetError(),
	//				Logger::RemovePathFromFile(__FILE__), __LINE__);
	//			SDL_FreeSurface(surface);
	//			return false;
	//		}
	//		else
	//		{
	//			int width = 0;
	//			int height = 0;
	//			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	//			size = { static_cast<float>(width), static_cast<float>(height) };
	//			SDL_FreeSurface(surface);
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		Logger::Log("Error: failed to load file " + path,
	//			Logger::RemovePathFromFile(__FILE__), __LINE__);
	//		return false;
	//	}


	//	return false;
	//}

	void Sprite::Update(float deltaTime)
	{
		// If sprite has more than one frame, its animated
		if (frames > 1)
		{
			source.x = (currentFrame % columns) * size.x;
			source.y = (currentFrame / columns) * size.y;

			if (timeAccumulator > timePerFrame)
			{
				timeAccumulator = 0;
				currentFrame++;
				if (currentFrame >= columns * rows)
				{
					currentFrame = 0;
				}
			}
			timeAccumulator += deltaTime;
		}
	}

	bool Sprite::LoadImage(SDL_Renderer* renderer, const std::string& path, unsigned int columns, unsigned int rows, unsigned int frames)
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
				this->columns = columns;
				this->rows = rows;
				this->frames = frames;
				int width = 0;
				int height = 0;
				SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
				width /= columns;
				height /= rows;
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
		destination = { static_cast<int>(finalPosition.x - (size.x / 2)), static_cast<int>(finalPosition.y - (size.y / 2)),
		static_cast<int>(size.x), static_cast<int>(size.y) };
		source = { 0,0,static_cast<int>(size.x), static_cast<int>(size.y) };
		if (frames > 1)
		{
			source.x = (currentFrame % columns) * size.x;
			source.y = (currentFrame / columns) * size.y;
		}

		SDL_RenderCopy(camera->GetInternalRenderer(), texture, &source, &destination);
	}

	

	void Sprite::Render(Camera* camera, Vector2D& position, char alpha)
	{
		SDL_SetTextureAlphaMod(texture, alpha);
		Render(camera, position);
		SDL_SetTextureAlphaMod(texture, 255);


	}
}





