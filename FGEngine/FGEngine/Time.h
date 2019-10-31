#pragma once

#include <SDL_stdinc.h>

namespace FG
{
	class Time
	{
	public:
		float DeltaTime() { return deltaTime * timeScale; }

		float GetTimeScale() const { return timeScale; }
		float SetTimeScale(float timeScale) { this->timeScale = timeScale; }

		void StartFrame();
		void EndFrame();

	private:
		Uint64 startTime = 0;
		Uint64 endTime = 0;
		float timeScale = 1.0f;

		float deltaTime = 0.0f;
	};
}
