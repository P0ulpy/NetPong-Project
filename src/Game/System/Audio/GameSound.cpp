#include "GameSound.hpp"

GameSound::GameSound(std::string name, std::string fullFileName)
	: _name(std::move(name)), _fullFileName(std::move(fullFileName))
{
	_soundClip = std::make_unique<sf::Sound>();
	_soundBuffer = std::make_unique<sf::SoundBuffer>();
}

void GameSound::setVolume(float newVolume)
{
	_soundClip->setVolume(newVolume);
}

const std::string& GameSound::getName() const
{
	return _name;
}

const std::string& GameSound::getFullFileName() const
{
	return _fullFileName;
}

sf::Sound* GameSound::getClip() const
{
	return _soundClip.get();
}

sf::SoundBuffer* GameSound::getSoundBuffer() const
{
	return _soundBuffer.get();
}