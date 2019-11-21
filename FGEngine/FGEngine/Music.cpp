#include "Music.h"

void FG::Music::Dispose()
{
	if (music)
		Mix_FreeMusic(music);
}

FG::Music::Music(std::string filename)
{
	music = Mix_LoadMUS(filename.c_str());
	if (music == NULL)
		printf("Music Loading Error: File-%s Error%s", filename.c_str(), Mix_GetError());
}
