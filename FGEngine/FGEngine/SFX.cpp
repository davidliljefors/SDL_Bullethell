#include "SFX.h"

void FG::SFX::Dispose()
{
	if (chunk)
		Mix_FreeChunk(chunk);
}

FG::SFX::SFX(std::string filename)
{
	chunk = Mix_LoadWAV(filename.c_str());
	if (chunk == nullptr)
		printf("SFX Loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
}
