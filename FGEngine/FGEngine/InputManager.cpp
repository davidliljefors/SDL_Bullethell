#include "InputManager.h"

#include <SDL_events.h>
#include <SDL_timer.h>

namespace FG
{
	void InputManager::Initialize()
	{
		keys = SDL_GetKeyboardState(nullptr);
		lastKeys = keys;
	}

	void InputManager::Update(bool& outQuit)
	{
		SDL_Event event;
		lastKeys = keys;
		keys = SDL_GetKeyboardState(nullptr);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				mousePosition.x = (float)event.motion.x;
				mousePosition.y = (float)event.motion.y;
				mouseMovement.x = (float)event.motion.xrel;
				mouseMovement.y = (float)event.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button] = true;
				mouseButtonTimes[event.button.button] = SDL_GetTicks();
				break;
			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button] = false;
				mouseButtonTimes[event.button.button] = 0;
				break;
			case SDL_QUIT:
				outQuit = true;
				break;
			}
		}
	}

	bool InputManager::IsKeyPressed(SDL_Scancode key) const
	{
		if (!lastKeys[key] && keys[key])
		{ return true; }
		return false;
	}

	bool InputManager::IsKeyReleased(SDL_Scancode key) const
	{
		if (lastKeys[key] && !keys[key])
		{ return true; }
		return false;
	}

	float InputManager::ElapsedKeyTime(SDL_Scancode key) const
	{
		Uint32 now = SDL_GetTicks();
		return (now - keyTimes[key]) * 0.001f;
	}

	bool InputManager::IsMousePressed(unsigned int mouseButton) const
	{
		if (!lastMouseButtons[mouseButton] && mouseButtons[mouseButton])
		{ return true; }
		return false;
	}

	bool InputManager::IsMouseReleased(unsigned int mouseButton) const
	{
		if (lastMouseButtons[mouseButton] && !mouseButtons[mouseButton])
		{ return true; }
		return false;
	}

	float InputManager::ElapsedMouseButtonTime(unsigned int mouseButton) const
	{
		Uint32 now = SDL_GetTicks();
		return (now - mouseButtonTimes[mouseButton]) * 0.001f;
	}

}
