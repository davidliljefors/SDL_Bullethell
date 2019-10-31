#pragma once

#include <string>

struct SDL_Window;

namespace FG
{
	class Window
	{
	public:
		virtual ~Window() {}

		virtual bool Initialize(const std::string& title, int width, int height, bool fullscreen = false);
		virtual void Shutdown();

		void SetTitle(const std::string& title);
		void SetSize(int width, int height);

		SDL_Window* GetInternalWindow() const { return window; }

	protected:
		SDL_Window* window = nullptr;
	};
}
