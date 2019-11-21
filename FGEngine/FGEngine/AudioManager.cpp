#include "AudioManager.h"

AudioManager::AudioManager(FG::ResourceManager* resourceManager) : resourceManager(resourceManager)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		printf("Mixer Initialization Error: %s\n", Mix_GetError());
	else
		ChangeMusicVolume(.25f);
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


void AudioManager::ChangeSFXVolume(std::string filename, float volume)
{
	Mix_VolumeChunk(Mix_LoadWAV(filename.c_str()), volume * MIX_MAX_VOLUME);
}

void AudioManager::ChangeMusicVolume(float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void AudioManager::ChangeChannelVolume(float volume, int channel)
{
	Mix_Volume(channel, volume * MIX_MAX_VOLUME);
}

float AudioManager::SFXVolume()
{
	return Mix_Volume(-1, -1);
}

float AudioManager::MusicVolume()
{
	return Mix_VolumeMusic(-1);
}
