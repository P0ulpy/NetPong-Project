#include "PhantomBall.hpp"
#include "PongBall.hpp"

PhantomBall::PhantomBall(PongBall& ballParent)
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
	_currentAlpha = 0;// std::floor(static_cast<float>(_initialAlpha) * (1 - _pongBallParent.getCurrentBoostTimeStamp()));
	_alphaDecreaseCooldown = 0.05f;
}

void PhantomBall::initShapeAndColor()
{
	_phantomBallShape = _pongBallParent.getShape();

	_currentFillColor = _phantomBallShape.getFillColor();
	_currentOutlineColor = _phantomBallShape.getOutlineColor();

	hidePhantomBall();
}

void PhantomBall::resetTimer()
{
	_timeAlphaDecreaseCooldown = 0;
}

void PhantomBall::update(const float& deltaTime)
{
	_timeAlphaDecreaseCooldown += deltaTime;

	if (_currentAlpha <= 10)
	{
		hidePhantomBall();
		//_pongBallParent.activatedErasingLastPhantomBall();
	}
	else if (_timeAlphaDecreaseCooldown > _alphaDecreaseCooldown)
	{
		_currentAlpha -= _alphaDecreaseSpeed;

		_currentFillColor.a = _currentAlpha;
		_currentOutlineColor.a = _currentAlpha;

		_phantomBallShape.setFillColor(_currentFillColor);
		_phantomBallShape.setOutlineColor(_currentOutlineColor);

		_timeAlphaDecreaseCooldown = 0;
	}
}

void PhantomBall::render(sf::RenderTarget& target) const
{
	target.draw(_phantomBallShape);
}

void PhantomBall::hidePhantomBall()
{
	_currentAlpha = 0;

	_currentFillColor.a = _currentAlpha;
	_currentOutlineColor.a = _currentAlpha;

	_phantomBallShape.setFillColor(_currentFillColor);
	_phantomBallShape.setOutlineColor(_currentOutlineColor);

	_isDisplayed = false;
}

void PhantomBall::displayPhantomBall()
{
	_currentAlpha = _initialAlpha;

	_currentFillColor.a = _currentAlpha;
	_currentOutlineColor.a = _currentAlpha;

	_phantomBallShape.setFillColor(_currentFillColor);
	_phantomBallShape.setOutlineColor(_currentOutlineColor);

	_phantomBallShape.setPosition(_pongBallParent.getShape().getPosition().x, _pongBallParent.getShape().getPosition().y);

	_isDisplayed = true;

	resetTimer();
}

bool PhantomBall::isDisplayed() const
{
	return _isDisplayed;
}