#include "PhantomBall.hpp"

#include "PongBall.hpp"
#include "../Effects/PhantomBallEffect.hpp"

constexpr int INITIAL_ALPHA = 160;
constexpr int ALPHA_DECREASE_SPEED = 20;//ALPHA_DECREASE_SPEED doit ętre un INITIAL_ALPHA de initialAlpha

constexpr float DURATION_BETWEEN_ALPHA_DECREASE = 0.05f;

PhantomBall::PhantomBall(PhantomBallEffect* phantomBallEffectParent, bool isActive)
	: _phantomBallEffectParent(phantomBallEffectParent)
{
	initVariables();
	initShapeAndColor();
	resetTimer();

	setIsActive(false);
}

PhantomBall::~PhantomBall() = default;

void PhantomBall::initVariables()
{
	_currentAlpha = 0;
}

void PhantomBall::initShapeAndColor()
{
	_phantomBallShape = _phantomBallEffectParent->getPongBallParent().getShape();

	_currentFillColor = _phantomBallShape.getFillColor();
	_currentOutlineColor = _phantomBallShape.getOutlineColor();
}

void PhantomBall::update(const float& deltaTime)
{
	_timeAlphaDecreaseCooldown += deltaTime;

	if (_currentAlpha <= 10)
	{
		setIsActive(false);
	}
	else if (_timeAlphaDecreaseCooldown > DURATION_BETWEEN_ALPHA_DECREASE)
	{
		setAndUpdateShapeAlpha(_currentAlpha < ALPHA_DECREASE_SPEED ? 0 : _currentAlpha - ALPHA_DECREASE_SPEED);

		_timeAlphaDecreaseCooldown = 0;
	}
}

void PhantomBall::render(sf::RenderTarget& target) const
{
	target.draw(_phantomBallShape);
}

void PhantomBall::setIsActive(bool isActive)
{
	_isActive = isActive;

	if(_isActive)
	{
		show();
	}
	else
	{
		hide();
	}
}

void PhantomBall::hide()
{
	setAndUpdateShapeAlpha(0);
}

void PhantomBall::setFillColor(const sf::Color& newColor)
{
	_currentFillColor = newColor;
}

void PhantomBall::show()
{
	setAndUpdateShapeAlpha(INITIAL_ALPHA);

	_phantomBallShape.setPosition(_phantomBallEffectParent->getPongBallParent().getShape().getPosition().x, 
									_phantomBallEffectParent->getPongBallParent().getShape().getPosition().y);

	resetTimer();
}

// GETTERS - SETTERS
void PhantomBall::setAndUpdateShapeAlpha(const sf::Uint8& alpha)
{
	_currentAlpha = alpha;

	_currentFillColor.a = _currentAlpha;
	_currentOutlineColor.a = _currentAlpha;

	_phantomBallShape.setFillColor(_currentFillColor);
	_phantomBallShape.setOutlineColor(_currentOutlineColor);
}

void PhantomBall::resetTimer()
{
	_timeAlphaDecreaseCooldown = 0;
}


bool PhantomBall::isActive() const
{
	return _isActive;
}