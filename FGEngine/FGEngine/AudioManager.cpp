#include "AudioManager.h"
#include "ResourceManager.h"
#include "Music.h"
#include "SFX.h"

AudioManager::AudioManager(FG::ResourceManager* resourceManager) : resourceManager(resourceManager)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		printf("Mixer Initialization Error: %s\n", Mix_GetError());
	else {
		ChangeMusicVolume(.25f);
		
		FG::Music* music = new FG::Music("QuartzQuadrantBad.wav");
		resourceManager->AddResource("QuartzQuadrantBad.wav", music);

		FG::SFX* sfx = new FG::SFX("fire.wav");
		resourceManager->AddResource("fire.wav", sfx);

		sfx = new FG::SFX("hit.wav");
		resourceManager->AddResource("hit.wav", sfx);
		sfx = new FG::SFX("hit1.wav");
		resourceManager->AddResource("hit1.wav", sfx);

		sfx = new FG::SFX("enemyFire.wav");
		resourceManager->AddResource("enemyFire.wav", sfx);
		sfx = new FG::SFX("enemyFire1.wav");
		resourceManager->AddResource("enemyFire1.wav", sfx);
		sfx = new FG::SFX("enemyFire2.wav");
		resourceManager->AddResource("enemyFire2.wav", sfx);

		sfx = new FG::SFX("enemyHurt.wav");
		resourceManager->AddResource("enemyHurt.wav", sfx);
		sfx = new FG::SFX("hitAlarm.wav");
		resourceManager->AddResource("hitAlarm.wav", sfx);
		sfx = new FG::SFX("playerDestroyed.wav");
		resourceManager->AddResource("playerDestroyed.wav", sfx);

	}
}

AudioManager::~AudioManager()
{
	Mix_Quit();
}

void AudioManager::PlaySFX(std::string filename, int channel, float volume, int loops)
{
	Mix_PlayChannel(channel, resourceManager->GetResource<FG::SFX>(filename)->chunk, loops);
}

void AudioManager::PlayMusic(std::string filename, int loops)
{
	Mix_PlayMusic(resourceManager->GetResource<FG::Music>(filename)->music, loops);
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
