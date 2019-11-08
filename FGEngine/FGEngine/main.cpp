#include "Application.h"
#include "Logger.h"

#include <cassert>
#include <SDL_main.h>

extern FG::Application* FG::CreateApplication();

int main(int argc, char* argv[])
{
	SDL_LogSetOutputFunction(&FG::Logger::LogSDL, nullptr);
	SDL_SetMainReady();
	FG::Application* application = FG::CreateApplication();
	assert(application);

	if (!application->Initialize())
	{
		application->Shutdown();
		delete application;
		application = nullptr;
		return -1;
	}

	application->Run();
	application->Shutdown();
	delete application;
	application = nullptr;

	return 0;
}
