#include "RoundStartCountdown.hpp"

#include <iostream>

constexpr float DURATION_BETWEEN_START_COUNTDOWN_NUMBERS = 0.6f;

RoundStartCountdown::RoundStartCountdown()
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

void RoundStartCountdown::update(const float& deltaTime)
{
	if (_isStartCountdownActive)
	{
		_currentTimeBetweenStartCountdownNumbers += deltaTime;
		if (_currentTimeBetweenStartCountdownNumbers >= DURATION_BETWEEN_START_COUNTDOWN_NUMBERS)
		{
			_currentTimeBetweenStartCountdownNumbers = 0;
			_currentCountdownMessageIndex++;

			if (_currentCountdownMessageIndex >= _countDownMessages.size())
			{
				stopBeginCountdown();
			}
		}
	}
}

void RoundStartCountdown::render(sf::RenderTarget& target) const
{
	if (_isStartCountdownActive)
	{
		std::cout << _countDownMessages.at(_currentCountdownMessageIndex).data() << std::endl;
	}
}

void RoundStartCountdown::startBeginCountdown()
{
	_isStartCountdownActive = true;
}

void RoundStartCountdown::stopBeginCountdown()
{
	_currentTimeBetweenStartCountdownNumbers = 0;
	_currentCountdownMessageIndex = 0;
	_isStartCountdownActive = false;
}

