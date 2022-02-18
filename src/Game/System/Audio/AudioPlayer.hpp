#include "SFML/Audio.hpp"

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
	void changeSoundsVolume() const;

	bool verifyLoadSoundsIntoBuffer() const;
	sf::Sound* getSoundFromSoundName(const std::string& soundName) const;
};