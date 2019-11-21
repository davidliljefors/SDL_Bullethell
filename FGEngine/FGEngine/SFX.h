#pragma once
#include "IResource.h"
#include <string>
#include "SDL_mixer.h"

namespace FG
{
	class SFX : public IResource
	{
	public: 
		void Dispose() override;
		SFX(std::string filename);

		Mix_Chunk* chunk;
	};

}