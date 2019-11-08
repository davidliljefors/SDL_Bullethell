#pragma once

#include "Vector2D.h"

#include <SDL_pixels.h>
#include <SDL.h>
#include "Sprite.h"

struct SDL_Renderer;

namespace FG
{
	struct Sprite;
	class Window;
	class Camera
	{
	public:
		Vector2D position;

		virtual bool Initialize(const Window* window);
		virtual void Shutdown();

		virtual void StartRenderFrame();
		virtual void EndRenderFrame();

		void DrawSprite(Sprite* sprite);

		void SetColor(const SDL_Color& color);

		SDL_Renderer* GetInternalRenderer() const { return renderer; }

	protected:
		const Window* ownerWindow = nullptr;
		SDL_Renderer* renderer = nullptr;
	};
}
