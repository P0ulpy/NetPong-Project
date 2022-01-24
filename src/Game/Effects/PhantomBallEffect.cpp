#include "PhantomBallEffect.hpp"

#include "../../Logger/Logger.hpp"
#include "../Entities/PhantomBall.hpp"
#include "../Entities/PongBall.hpp"

constexpr int MAX_PHANTOM_BALLS = 20;
constexpr float DURATION_BETWEEN_PHANTOM_BALLS = .025f;

PhantomBallEffect::PhantomBallEffect(const PongBall& ballParent)
	: _ballParent(ballParent)
{
	createPhantomBalls();
	begin();
}

PhantomBallEffect::~PhantomBallEffect()
{
}

void PhantomBallEffect::update(const float& deltaTime)
{
	if (_hasPhantomEffect)
	{
		_currentTimePhantomBallCooldown += deltaTime;

		if (_currentTimePhantomBallCooldown > DURATION_BETWEEN_PHANTOM_BALLS)
		{
			displayPhantomBall();
			_currentTimePhantomBallCooldown = 0;
		}
	}

	//Even if there is no phantomEffect, we update phantomBalls that are still displayed on the screen
	for (const auto& phantomBall : _phantomBalls)
	{
		if (phantomBall->isDisplayed())
		{
			phantomBall->update(deltaTime);
		}
	}
}

void PhantomBallEffect::render(sf::RenderTarget& target) const
{
	for (const auto& phantomBall : _phantomBalls)
	{
		if (phantomBall->isDisplayed())
		{
			phantomBall->render(target);
		}
	}
}

void PhantomBallEffect::begin()
{
	_hasPhantomEffect = true;
	_currentTimePhantomBallCooldown = 0;
}

void PhantomBallEffect::stop()
{
	_hasPhantomEffect = false;
}

void PhantomBallEffect::displayPhantomBall()
{
	bool isPhantomBallDisplayed = false;

	//Recherche de la premiere PhantomBall qui n'est pas affichée dans la liste
	for (const auto& phantomBall : _phantomBalls)
	{
		if (!phantomBall->isDisplayed())
		{
			phantomBall->show();
			isPhantomBallDisplayed = true;
			break;
		}
	}

	if (!isPhantomBallDisplayed)
	{
		Logger::Log("ERROR PongBall.cpp | displayPhantomBall() : pas assez de _phantomBallsMax ! ");
	}
}

void PhantomBallEffect::createPhantomBalls()
{
	for (int i = 0; i < MAX_PHANTOM_BALLS; i++)
	{
		_phantomBalls.push_back(std::make_unique<PhantomBall>(_ballParent));
	}
}

void PhantomBallEffect::setPhantomBallFillColour(const sf::Color& newColor) const
{
	for (const auto& phantomBall : _phantomBalls)
	{
		phantomBall->setFillColor(newColor);
	}
}
