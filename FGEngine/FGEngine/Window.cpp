#include "Window.h"
#include "Logger.h"

#include <SDL_video.h>

namespace FG
{
	bool Window::Initialize(const std::string& title, int width, int height)
	{
		window = SDL_CreateWindow(title.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, SDL_WINDOW_SHOWN);
		if (!window)
		{
			FG::Logger::Log(SDL_GetError(), FG::Logger::RemovePathFromFile(__FILE__), __LINE__);
			return false;
		}

		return true;
	}

	void Window::Shutdown()
	{
		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}
	}

	void Window::SetTitle(const std::string& title)
	{
		SDL_SetWindowTitle(window, title.c_str());
	}

	void Window::SetSize(int width, int height)
	{
		SDL_SetWindowSize(window, width, height);
	}
}
