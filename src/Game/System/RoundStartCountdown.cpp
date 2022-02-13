#include "RoundStartCountdown.hpp"

#include "../../Engine/Scenes/MainGameScene.hpp"

#include <iostream>

#include "Audio/AudioPlayer.hpp"

constexpr float DURATION_BETWEEN_START_COUNTDOWN_NUMBERS = 0.6f;

RoundStartCountdown::RoundStartCountdown(const MainGameScene& mainGameScene)
	: _mainGameScene(mainGameScene)
{
	initValues();
}

RoundStartCountdown::~RoundStartCountdown()
{
}

void RoundStartCountdown::initValues()
{
	_countDownMessages = { "3", "2", "1", "GO!" };
	_currentCountdownMessageIndex = 0;

	stopBeginCountdown();
}

void RoundStartCountdown::displayNextMessage() const
{
	std::cout << _countDownMessages.at(_currentCountdownMessageIndex).data() << std::endl;
}

void RoundStartCountdown::update(const float& deltaTime)
{
	if (_isStartCountdownActive)
	{
		_currentTimeBetweenStartCountdownNumbers += deltaTime;
		if (_currentTimeBetweenStartCountdownNumbers >= DURATION_BETWEEN_START_COUNTDOWN_NUMBERS)
		{
			_currentTimeBetweenStartCountdownNumbers = 0;
			_currentCountdownMessageIndex++;

			if (_currentCountdownMessageIndex >= _countDownMessages.size()-1)
			{
				displayNextMessage();
				_mainGameScene.getAudioPlayer()->playSound("Go");

				_mainGameScene.togglePlayersMovement(true);

				stopBeginCountdown();
			}
			else
			{
				displayNextMessage();
				_mainGameScene.getAudioPlayer()->playSound("Ready");
			}
		}
	}
}

void RoundStartCountdown::render(sf::RenderTarget& target) const
{
	if (_isStartCountdownActive)
	{
		//DisplayInsaneCountdownUI
	}
}

void RoundStartCountdown::startBeginCountdown()
{
	displayNextMessage();
	_mainGameScene.getAudioPlayer()->playSound("Ready");
	_isStartCountdownActive = true;
}

void RoundStartCountdown::stopBeginCountdown()
{
	_currentTimeBetweenStartCountdownNumbers = 0;
	_currentCountdownMessageIndex = 0;
	_isStartCountdownActive = false;
}

