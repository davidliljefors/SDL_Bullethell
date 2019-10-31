#include "Time.h"

#include <SDL_timer.h>

namespace FG
{
	void Time::StartFrame()
	{
		startTime = SDL_GetPerformanceCounter();
	}

	void Time::EndFrame()
	{
		endTime = SDL_GetPerformanceCounter();
		deltaTime = (endTime - startTime) / (float)SDL_GetPerformanceFrequency();
	}
}
