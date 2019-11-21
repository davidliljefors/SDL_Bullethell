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

	class Text : public IResource
	{
		friend class Animation;
	public:
		Vector2D size;

		void Dispose() override;
		bool SetText(SDL_Renderer* renderer, std::string text, const std::string& path, int size, SDL_Color color = {255,255,255});
		void Render(Camera* camera, Vector2D position, const SDL_Rect* src = nullptr);
		void Render(Camera* camera, Vector2D position, char alpha, const SDL_Rect* src = nullptr);


	protected:
		SDL_Texture* texture = nullptr;
		SDL_Color color;
		SDL_Rect destination;

	};
}