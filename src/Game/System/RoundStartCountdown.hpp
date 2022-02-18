#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderTarget.hpp"


class MainGameScene;
class UI;

class RoundStartCountdown
{
private:
	const MainGameScene& _mainGameScene;
	std::vector<std::string> _countDownMessages;
	int _currentCountdownMessageIndex;
	float _currentTimeBetweenStartCountdownNumbers;
	bool _isStartCountdownActive;
	std::unique_ptr<UI> _UI;
	void initValues();
	void displayNextMessage() const;
	sf::Font font;

public:
	RoundStartCountdown(const MainGameScene& mainGameScene);
	~RoundStartCountdown();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void startBeginCountdown();
	void stopBeginCountdown();
};
