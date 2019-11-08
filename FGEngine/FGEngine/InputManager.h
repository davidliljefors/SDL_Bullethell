#pragma once

#pragma warning( push )
#pragma warning (disable : 26812 )

#include "Vector2D.h"

#include <SDL_mouse.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>

namespace FG
{
	class InputManager
	{
	public:
		Vector2D mousePosition;
		// Delta movement since last update.
		Vector2D mouseMovement;

		void Initialize();
		void Update(bool& shouldQuit);

		bool IsKeyDown(SDL_Scancode key) const { return keys[key]; }
		bool IsKeyPressed(SDL_Scancode key) const;
		bool IsKeyReleased(SDL_Scancode key) const;

		float ElapsedKeyTime(SDL_Scancode key) const;

		bool IsMouseButtonDown(unsigned int mouseButton) const { return mouseButtons[mouseButton]; }
		bool IsMouseButtonPressed(unsigned int mouseButton) const;
		bool IsMouseButtonReleased(unsigned int mouseButton) const;

		float ElapsedMouseButtonTime(unsigned int mouseButton) const;

	private:
		const Uint8* keys = nullptr;
		bool mouseButtons[SDL_BUTTON_X2] = { false };

		const Uint8* lastKeys = nullptr;
		bool lastMouseButtons[SDL_BUTTON_X2] = { false };

		unsigned int keyTimes[SDL_NUM_SCANCODES] = { 0 };
		unsigned int mouseButtonTimes[SDL_BUTTON_X2] = { 0 };
	};
}
#pragma warning( pop )
