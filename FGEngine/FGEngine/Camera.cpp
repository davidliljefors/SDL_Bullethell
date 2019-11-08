#include "Camera.h"
#include "Logger.h"
#include "Window.h"

#include <assert.h>
#include <SDL_render.h>

namespace FG
{
	bool Camera::Initialize(const Window* window)
	{
		assert(window);
		ownerWindow = window;
		renderer = SDL_CreateRenderer(window->GetInternalWindow(), -1, SDL_RENDERER_ACCELERATED);
		if (!renderer)
		{
			FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
			return false;
		}

		return true;
	}

	void Camera::Shutdown()
	{
		if (renderer)
		{
			SDL_DestroyRenderer(renderer);
			renderer = nullptr;
		}
	}

	void Camera::StartRenderFrame()
	{
		SDL_RenderClear(renderer);
	}

	void Camera::EndRenderFrame()
	{
		SDL_RenderPresent(renderer);
	}

	void Camera::DrawSprite(Sprite* sprite)
	{
		sprite->dst.x = sprite->parent->GetPosition().x - (sprite->dst.w / 2);
		sprite->dst.y = sprite->parent->GetPosition().y - (sprite->dst.h / 2);
		SDL_RenderCopyEx(renderer, sprite->tex, &sprite->src, &sprite->dst, NULL, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	void Camera::SetColor(const SDL_Color& color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	}
}
