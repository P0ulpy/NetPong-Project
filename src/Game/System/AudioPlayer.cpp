#include "AudioPlayer.hpp"

#include <iostream>

AudioPlayer::AudioPlayer()
{
	initGameSounds();
}

AudioPlayer::~AudioPlayer()
{ }

void AudioPlayer::playSound(const std::string& soundName) const
{
	if(sf::Sound* s; ( s = getSoundFromSoundName(soundName) ) != nullptr)
	{
		s->play();
	}
}

void AudioPlayer::initGameSounds()
{
	_sounds.push_back(new GameSound("Button", "./Assets/Sounds/Button.wav"));

	_sounds.push_back(new GameSound("Explosion", "./Assets/Sounds/Explosion.wav"));
	_sounds.push_back(new GameSound("Bounce", "./Assets/Sounds/Bounce.wav"));
	_sounds.push_back(new GameSound("Shoot", "./Assets/Sounds/Shoot.wav"));
	_sounds.push_back(new GameSound("Ready", "./Assets/Sounds/Ready.wav"));
	_sounds.push_back(new GameSound("Go", "./Assets/Sounds/Go.wav"));


	if (!verifyLoadSoundsIntoBuffer())
	{
		std::cout << "Error AudioPlayer : some sounds couldn't have been loaded !";
	}
}

bool AudioPlayer::verifyLoadSoundsIntoBuffer() const
{
	for (const auto sound : _sounds)
	{
		if (!sound->getSoundBuffer()->loadFromFile(sound->getFullFileName()))
		{
			return false;
		}

		sound->getClip()->setBuffer(*sound->getSoundBuffer());
	}

	return true;
}

sf::Sound* AudioPlayer::getSoundFromSoundName(const std::string& soundName) const
{
	for (const auto sound : _sounds)
	{
		if (sound->getName() == soundName)
		{
			return sound->getClip();
		}
	}

	std::cout << "Error AudioPlayer.cpp : your sound is in another castle. " <<
				 "Be sure to call verifyLoadSoundsIntoBuffer() before play any sounds";

	return nullptr;
}
