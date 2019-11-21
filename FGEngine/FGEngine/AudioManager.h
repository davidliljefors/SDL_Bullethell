#pragma once

#include <SDL_mixer.h>
#include <string>

class AudioManager
{
public:

	AudioManager();
	~AudioManager();

	void PlaySFX(std::string filename, int channel = 0, float volume = 1, int loops = 0);
	void PlayMusic(std::string filename, int loops = -1);

	void PauseMusic();
	void ResumeMusc();

private:

};