#include "AudioManager.h"

#if USE_EXPERIMENTAL_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
#else
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;
#endif

AudioManager::AudioManager() : soundsVolume(1.0f), musicVolume(1.0f), backgroundMusic(-1), soundExtension(".mp3"), musicExtension(".mp3")
{
    
}

AudioManager & AudioManager::getInstance()
{
	static AudioManager instance;

	return instance;
}

void AudioManager::setSoundsVolume(float volume)
{
	soundsVolume = volume;
    
#if USE_EXPERIMENTAL_AUDIO_ENGINE
    
#else
    SimpleAudioEngine::getInstance()->setEffectsVolume(soundsVolume);
#endif
}

void AudioManager::setMusicVolume(float volume)
{
	musicVolume = volume;
	
	if (backgroundMusic > -1)
	{
#if USE_EXPERIMENTAL_AUDIO_ENGINE
		AudioEngine::setVolume(backgroundMusic, musicVolume);
#else
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
#endif
	}
}

float AudioManager::getMusicVolume()
{
	return musicVolume;
}

int AudioManager::PlaySoundEffect(std::string name, bool loop /*= false*/)
{
	if (soundsVolume < 0.01f)
		return -1;
    
#if USE_EXPERIMENTAL_AUDIO_ENGINE
    return AudioEngine::play2d(name + soundExtension, loop, soundsVolume);
#else
    SimpleAudioEngine::getInstance()->setEffectsVolume(soundsVolume);
    
    auto soundid = SimpleAudioEngine::getInstance()->playEffect((name + soundExtension).c_str(), loop);
    
    return soundid;
#endif

	
}

void AudioManager::StopSoundEffect(int soundID)
{
#if USE_EXPERIMENTAL_AUDIO_ENGINE
    AudioEngine::stop(soundID);
#else
    SimpleAudioEngine::getInstance()->stopEffect(soundID);
#endif
}

void AudioManager::setSoundsExtension(std::string extension)
{
	soundExtension = extension;
}

void AudioManager::setMusicExtension(std::string extension)
{
	musicExtension = extension;
}

void AudioManager::PlayBackgroundMusic(std::string name)
{
	StopBackgroundMusic();
    lastBackgroundMusic = name;
    
    if (musicVolume > 0.01f)
    {
#if USE_EXPERIMENTAL_AUDIO_ENGINE
        backgroundMusic = AudioEngine::play2d(name + musicExtension, true, musicVolume);
#else
        SimpleAudioEngine::getInstance()->playBackgroundMusic((name + musicExtension).c_str(), true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(musicVolume);
        backgroundMusic = 0;
#endif
        
    }
}

void AudioManager::StopBackgroundMusic()
{
	if (backgroundMusic > -1)
	{
#if USE_EXPERIMENTAL_AUDIO_ENGINE
        AudioEngine::stop(backgroundMusic);
        backgroundMusic = -1;
#else
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        backgroundMusic = -1;
#endif
	}
}

void AudioManager::PauseBackgroundMusic()
{
	if (backgroundMusic > -1)
	{
#if USE_EXPERIMENTAL_AUDIO_ENGINE
        AudioEngine::pause(backgroundMusic);
#else
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
	}
}

void AudioManager::ResumeBackgroundMusic()
{
    PlayBackgroundMusic(lastBackgroundMusic);
	/*if (backgroundMusic > -1)
	{
		if (!AudioEngine::resume(backgroundMusic))
        {
            PlayBackgroundMusic(lastBackgroundMusic);
        }
	}*/
}
