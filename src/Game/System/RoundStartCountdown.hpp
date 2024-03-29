#pragma once
#include <string>
#include <vector>

#include "SFML/Graphics/RenderTarget.hpp"

class MainGameScene;

class RoundStartCountdown
{
private:
	const MainGameScene& _mainGameScene;

	std::vector<std::string> _countDownMessages;
	int _currentCountdownMessageIndex;
	float _currentTimeBetweenStartCountdownNumbers;
	bool _isStartCountdownActive;

	void initValues();
	void displayNextMessage() const;

public:
	RoundStartCountdown(const MainGameScene& mainGameScene);
	~RoundStartCountdown();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void startBeginCountdown();
	void stopBeginCountdown();
};
