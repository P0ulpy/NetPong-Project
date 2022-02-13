#include "SFML/Audio.hpp"

#include <map>

class GameSound;

class AudioPlayer
{
public:
	AudioPlayer();
	~AudioPlayer();

	void playSound(const std::string& soundName) const;

private:
	std::vector<GameSound*> _sounds;

	const std::string& _soundFileExtension { ".wav" };

	void initGameSounds();
	bool verifyLoadSoundsIntoBuffer() const;
	sf::Sound* getSoundFromSoundName(const std::string& soundName) const;
};

class GameSound
{
private:
	std::string _name;
	std::string _fullFileName;
	std::unique_ptr<sf::Sound> _soundClip;
	std::unique_ptr<sf::SoundBuffer> _soundBuffer;

public:
	GameSound(std::string name, std::string fullFileName)
		: _name(std::move(name)), _fullFileName(std::move(fullFileName))
	{
		_soundClip = std::make_unique<sf::Sound>();
		_soundBuffer = std::make_unique<sf::SoundBuffer>();
	}

	const std::string& getName() const
	{
		return _name;
	}

	const std::string& getFullFileName() const
	{
		return _fullFileName;
	}

	sf::Sound* getClip() const
	{
		return _soundClip.get();
	}

	sf::SoundBuffer* getSoundBuffer() const
	{
		return _soundBuffer.get();
	}
};