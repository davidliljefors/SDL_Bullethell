#pragma once
#include "IResource.h"
#include "Vector2D.h"
#include "SDL_rect.h"

#include <string>

struct SDL_Texture;
struct SDL_Renderer;

namespace FG
{
	class Camera;

	class Sprite : public IResource
	{
		friend class Animation;
	public:
		Vector2D size;

		void Dispose() override;
		bool LoadImage(SDL_Renderer* renderer, const std::string& path,
			unsigned int columns = 1, unsigned int rows = 1, unsigned int frames = 1);
		void Render(Camera* camera, Vector2D& position, const SDL_Rect* src = nullptr);
		void Render(Camera* camera, Vector2D& position, char alpha, const SDL_Rect* src = nullptr);

		unsigned int GetFrameCount()
		{
			return frames;
		}

	protected:
		SDL_Texture* texture = nullptr;
		SDL_Rect destination;
	private:
		unsigned int frames = 1;
		unsigned int columns = 1;
		unsigned int rows = 1;
		//Frames to show each sprite
		float timePerFrame = 0.1f;

	};
}

