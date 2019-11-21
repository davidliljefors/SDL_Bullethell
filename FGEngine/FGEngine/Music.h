#pragma once
#include "IResource.h"
#include <string>
#include "SDL_mixer.h"

namespace FG
{
	class Music : public IResource
	{
	public:
		void Dispose() override;
		Music(std::string filename);

		Mix_Music* music;
	};

}