#include "PhantomBall.hpp"
#include "PongBall.hpp"

PhantomBall::PhantomBall(const PongBall& ballParent)
	: _pongBallParent(ballParent)
{
	initVariables();
	initShapeAndColor();
	resetTimer();
}

PhantomBall::~PhantomBall()
{

}

void PhantomBall::initVariables()
{
	_alphaDecreaseSpeed = 20;//_alphaDecreaseSpeed doit ętre un multiple de _initialAlpha
	_initialAlpha = 160;
	_currentAlpha = 0;
	_alphaDecreaseCooldown = 0.05f;
}

void PhantomBall::initShapeAndColor()
{
	_phantomBallShape = _pongBallParent.getShape();

	_currentFillColor = _phantomBallShape.getFillColor();
	_currentOutlineColor = _phantomBallShape.getOutlineColor();

	hide();
}

void PhantomBall::update(const float& deltaTime)
{
	_timeAlphaDecreaseCooldown += deltaTime;

	if (_currentAlpha <= 10)
	{
		hide();
	}
	else if (_timeAlphaDecreaseCooldown > _alphaDecreaseCooldown)
	{
		setAndUpdateShapeAlpha(_currentAlpha - _alphaDecreaseSpeed);

		_timeAlphaDecreaseCooldown = 0;
	}
}

void PhantomBall::render(sf::RenderTarget& target) const
{
	target.draw(_phantomBallShape);
}

void PhantomBall::hide()
{
	setAndUpdateShapeAlpha(0);

	_isDisplayed = false;
}

void PhantomBall::show()
{
	setAndUpdateShapeAlpha(_initialAlpha);

	_phantomBallShape.setPosition(_pongBallParent.getShape().getPosition().x, _pongBallParent.getShape().getPosition().y);
	_isDisplayed = true;

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


bool PhantomBall::isDisplayed() const
{
	return _isDisplayed;
}