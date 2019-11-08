#pragma once

#include "Vector2D.h"

#include <SDL_pixels.h>

struct SDL_Renderer;

namespace FG
{
	class Window;
	class Camera
	{
	public:
		Vector2D position;

		virtual bool Initialize(Window* window);
		virtual void Shutdown();

		virtual void StartRenderFrame();
		virtual void EndRenderFrame();

		void SetColor(const SDL_Color& color);
		SDL_Color GetColor();

		SDL_Renderer* GetInternalRenderer() const { return renderer; }

	protected:
		Window* ownerWindow = nullptr;
		SDL_Renderer* renderer = nullptr;
	};
}
