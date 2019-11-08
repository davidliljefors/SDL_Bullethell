#pragma once

#include <SDL_stdinc.h>

namespace FG
{
	class Time
	{
	public:

		float DeltaTime() const { return deltaTime * timeScale; }

		float& TimeScale() { return timeScale; }

		void StartFrame();
		void EndFrame();

	private:
		Uint64 startTime = 0;
		Uint64 endTime = 0;
		float timeScale = 1.0f;
		float deltaTime = 0.0f;
	};
}
