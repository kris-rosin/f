// revision 2

#pragma once

#include <string>

#define USE_EXPERIMENTAL_AUDIO_ENGINE false

class AudioManager
{
private:
	float soundsVolume;
	float musicVolume;

	std::string soundExtension;
	std::string musicExtension;
    std::string lastBackgroundMusic;
    
	int backgroundMusic;
    
private:
	AudioManager();
	AudioManager(const AudioManager& root) {};
	AudioManager& operator=(const AudioManager&) { return *this; };
public:
	static AudioManager & getInstance();

	~AudioManager() {};

	void setSoundsVolume(float volume);
	void setSoundsExtension(std::string extension);

	void setMusicVolume(float volume);
	float getMusicVolume();
	void setMusicExtension(std::string extension);

	int  PlaySoundEffect(std::string name, bool loop = false);
	void PlayBackgroundMusic(std::string name);
	void PauseBackgroundMusic();
	void ResumeBackgroundMusic();
	void StopBackgroundMusic();
	void StopSoundEffect(int soundID);
};

