#include "AudioManager.h"

AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("Mixer Initialization Error: %s\n", Mix_GetError());
	}
}

AudioManager::~AudioManager()
{
	Mix_Quit();
}

void AudioManager::PlaySFX(std::string filename, int channel, float volume, int loops)
{
	Mix_PlayChannel(channel, Mix_LoadWAV(filename.c_str()), loops);
}

void AudioManager::PlayMusic(std::string filename, int loops)
{
	Mix_PlayMusic(Mix_LoadMUS(filename.c_str()), loops);
}

void AudioManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

void AudioManager::ResumeMusc()
{
	if (Mix_PlayingMusic() != 0)
		Mix_ResumeMusic();
}
