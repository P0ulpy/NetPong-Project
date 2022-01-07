#pragma once
#include <string>
#include <vector>

#include "SFML/Graphics/RenderTarget.hpp"

class RoundStartCountdown
{
private:
	std::vector<std::string> _countDownMessages;
	int _currentCountdownMessageIndex;
	float _currentTimeBetweenStartCountdownNumbers;
	bool _isStartCountdownActive;

	void initValues();

public:
	RoundStartCountdown();
	~RoundStartCountdown();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void startBeginCountdown();
	void stopBeginCountdown();
};
