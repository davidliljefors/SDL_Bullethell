#include "Camera.h"
#include "Logger.h"
#include "Window.h"

#include <cassert>


bool FG::Camera::Initialize(Window* window, unsigned int flags)
{
	assert(window);
	ownerWindow = window;
	renderer = SDL_CreateRenderer(window->GetInternalWindow(), -1, flags);
	if (!renderer)
	{
		FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
		return false;
	}
	return true;
}

void FG::Camera::Shutdown()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

void FG::Camera::StartRenderFrame()
{
	SDL_RenderClear(renderer);
}

void FG::Camera::EndRenderFrame()
{
	SDL_RenderPresent(renderer);
}

void FG::Camera::SetColor(const SDL_Color& color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

SDL_Color FG::Camera::GetColor()
{
	Uint8 red = 0;
	Uint8 blue = 0;
	Uint8 green = 0;
	Uint8 alpha = 0;
	SDL_GetRenderDrawColor(renderer, &red, &green, &blue, &alpha);
	return { red, green, blue, alpha };
}
