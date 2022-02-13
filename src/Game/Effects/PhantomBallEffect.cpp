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

PhantomBallEffect::~PhantomBallEffect() = default;

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
		if (phantomBall->isActive())
		{
			phantomBall->update(deltaTime);
		}
	}
}

void PhantomBallEffect::render(sf::RenderTarget& target) const
{
	for (const auto& phantomBall : _phantomBalls)
	{
		phantomBall->render(target);
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

void PhantomBallEffect::displayPhantomBall() const
{
	bool inactiveBallFound = false;

	for (const auto phantomBall : _phantomBalls)
	{
		if(!phantomBall->isActive())
		{
			phantomBall->setIsActive(true);
			inactiveBallFound = true;
			break;
		}
	}

	if(!inactiveBallFound)
	{
		Logger::Log("ERROR PhantomBallEffect.cpp | displayPhantomBall() : pas assez de _phantomBallsMax ! ");
	}
}

void PhantomBallEffect::createPhantomBalls()
{
	for (int i = 0; i < MAX_PHANTOM_BALLS; i++)
	{
		_phantomBalls.emplace_back(new PhantomBall(this));
	}
}

void PhantomBallEffect::setPhantomBallFillColor(const sf::Color& newColor) const
{
	for (const auto& phantomBall : _phantomBalls)
	{
		phantomBall->setFillColor(newColor);
	}
}

const PongBall& PhantomBallEffect::getPongBallParent() const
{
	return _ballParent;
}
