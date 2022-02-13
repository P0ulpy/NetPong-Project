#include "SFML/Audio.hpp"

class GameSound
{
private:
	std::string _name;
	std::string _fullFileName;
	std::unique_ptr<sf::Sound> _soundClip;
	std::unique_ptr<sf::SoundBuffer> _soundBuffer;

public:
	GameSound(std::string name, std::string fullFileName);

	void setVolume(float newVolume);

	const std::string& getName() const;
	const std::string& getFullFileName() const;
	sf::Sound* getClip() const;
	sf::SoundBuffer* getSoundBuffer() const;
};