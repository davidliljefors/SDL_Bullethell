#pragma once

#include "Application.h"
#include "Logger.h"

#include <SDL_main.h>

extern FG::Application* FG::CreateApplication();

int main(int argc, char* argv[])
{
	SDL_LogSetOutputFunction(&FG::Logger::LogSDL, NULL);
	SDL_SetMainReady();
	FG::Application* application = FG::CreateApplication();

	if (!application->Initialize())
	{
		application->Shutdown();
		delete application;
		return -1;
	}

	application->Run();
	application->Shutdown();

	if (application)
	{
		application->Shutdown();
		delete application;
	}

	return 0;
}
