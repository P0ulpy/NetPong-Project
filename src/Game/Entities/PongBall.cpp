#include "PongBall.hpp"

#include <iostream>

#include "../../Engine/Scenes/MainGameScene.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Logger/Logger.hpp"
#include "../Effects/PhantomBallEffect.hpp"
#include "../System/Audio/AudioPlayer.hpp"

constexpr int BALL_SIZE = 25;

constexpr int MAX_NUM_BOUNCES = 2;
constexpr float BOOST_DURATION = 2.f;

constexpr float INITIAL_SPEED = 650.f;

//--- Constructors - Destructor ---
PongBall::PongBall(const sf::RenderWindow& window, MainGameScene& mainGameScene)
	: _mainGameScene(&mainGameScene), _polygonTerrain(mainGameScene.getPolygonTerrain())
{
	initVariables();
	initShapes(window);
	initBoost();

	_phantomBallEffect = std::make_unique<PhantomBallEffect>(*this);

	startPhantomBallEffect();

	setActive(false);
}

PongBall::~PongBall()
{ }

//--- Initializers ---
void PongBall::initVariables()
{
	_velocity = Utils::normalize(sf::Vector2f(1.f,1.5f));
	_initialSpeed = INITIAL_SPEED;
	_currentSpeed = _initialSpeed;
	_maxSpeed = 10000.f;

	_ballSize = BALL_SIZE;
	_ballColor = sf::Color(65, 90, 255, 255);
	_ballInactiveColor = sf::Color(200, 200, 200, 255);

	_speedMultiplierBonus = 1.f;
}

void PongBall::initShapes(const sf::RenderWindow& window)
{
	_ballShape.setRadius(static_cast<float>(_ballSize));
	_ballShape.setFillColor(_ballColor);
	_ballShape.setOutlineColor(sf::Color::White);
	_ballShape.setOutlineThickness(2);
	_ballShape.setOrigin(_ballShape.getGlobalBounds().width / 2.f, _ballShape.getGlobalBounds().height / 2.f);

	_initialPosition = sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f);
	_ballShape.setPosition(_initialPosition);

	_oldPosition = _ballShape.getPosition();

	//FOR DEBUG PURPOSES
	_ballDestination = new sf::CircleShape();
	_ballDestination->setRadius(5.f);
	_ballDestination->setFillColor(sf::Color::Red);
	_ballDestination->setOrigin(_ballDestination->getGlobalBounds().width / 2.f, _ballDestination->getGlobalBounds().height / 2.f);
	_ballDestination->setPosition(_ballShape.getPosition());
}

void PongBall::initBoost()
{
	_isBoosted = false;
	_currentTimeBoost = 0.f;
}

void PongBall::startPhantomBallEffect()
{
	_phantomBallEffect->begin();
}

//--- Updating ---
void PongBall::update(const float& deltaTime)
{
	if(_isActive)
	{
		updateBoost(deltaTime);
		updateMovement(deltaTime);
		updateCollision(deltaTime);
	}

	updatePhantomEffect(deltaTime);
}

void PongBall::updateCollision(const float& deltaTime)
{
	//TODO : très très moche
	if(!_ballShape.getGlobalBounds().intersects(sf::Rect<float>(0,0,1920,1080)))
	{
		Logger::Log("Out of window !");
		_ballShape.setPosition(_initialPosition);
	}
}

void PongBall::updateMovement(const float& deltaTime)
{
	_oldPosition = _ballShape.getPosition();
	moveEntity(_velocity, deltaTime);
	_ballDestination->setPosition(std::abs(_ballShape.getPosition().x) + Utils::normalize(_velocity).x * _currentSpeed * deltaTime,
		std::abs(_ballShape.getPosition().y) + Utils::normalize(_velocity).y * _currentSpeed * deltaTime);
}

void PongBall::updateBoost(const float& deltaTime)
{
	if (_isBoosted)
	{
		_currentTimeBoost += deltaTime;
		if (_currentTimeBoost >= BOOST_DURATION)
		{
			_isBoosted = false;
			resetSpeedMultiplierBonus();
		}
	}
}

void PongBall::updatePhantomEffect(const float& deltaTime)
{
	_phantomBallEffect->update(deltaTime);
}

//--- Rendering ---
void PongBall::render(sf::RenderTarget& target) const
{
	renderPhantomEffect(target);

	if(_isActive)
	{
		target.draw(_ballShape);
		//target.draw(*_ballDestination);
	}
}

void PongBall::renderPhantomEffect(sf::RenderTarget& target) const
{
	_phantomBallEffect->render(target);
}
 
bool PongBall::hitPlayer(float c2x, float c2y, float c2r, sf::Color color2) const
{
	if (_canKill && _isActive && color2 != _ballColor)
	{
		return Utils::circleCircleCollision(_ballShape.getPosition().x, _ballShape.getPosition().y, _ballShape.getRadius(), c2x, c2y, c2r);
	}

	return false;
}

bool PongBall::hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime)
{
	sf::Vector2f outImpactPoint{ 0,0 };

	bool hit = linePongBallCollision(x1, y1, x2, y2, outImpactPoint, remainingTime);

	if (hit)
	{
		const sf::Vector2f surfaceVector = Utils::normalize(sf::Vector2f(x2 - x1, y2 - y1));
		const auto normalSurfaceVector = sf::Vector2f(-surfaceVector.y, surfaceVector.x);

		_velocity = Utils::getVectorReflection(_velocity, surfaceVector);

		_ballShape.setPosition(outImpactPoint.x + normalSurfaceVector.x * _ballShape.getRadius() + _velocity.x * (remainingTime),
								outImpactPoint.y + normalSurfaceVector.y * _ballShape.getRadius() + _velocity.y * (remainingTime));

		_ballDestination->setPosition(std::abs(_ballShape.getPosition().x) + Utils::normalize(_velocity).x * _currentSpeed * deltaTime,
			std::abs(_ballShape.getPosition().y) + Utils::normalize(_velocity).y * _currentSpeed * deltaTime);

		return true;
	}

	return false;

}

void PongBall::shoot(sf::Vector2f position, sf::Vector2f normVelocity, const sf::Color& colorNormal, const sf::Color& colorInactive)
{
	_ballShape.setPosition(position);
	_velocity = normVelocity;
	_ballColor = colorNormal;
	_ballInactiveColor = colorInactive;

	Logger::Log("SHOOT BALL !");

	setActive(true);
}

void PongBall::resetBallDestAndOldPos(const float& deltaTime)
{
	_ballDestination->setPosition(std::abs(_ballShape.getPosition().x) + Utils::normalize(_velocity).x * _currentSpeed * deltaTime,
		std::abs(_ballShape.getPosition().y) + Utils::normalize(_velocity).y * _currentSpeed * deltaTime);

	_oldPosition = _ballShape.getPosition();
}

//--- Getters - Setters ---
void PongBall::setSpeedMultiplierBonus(float pSpeedMultiplierBonus)
{
	if (pSpeedMultiplierBonus * _currentSpeed < _maxSpeed)
	{
		_speedMultiplierBonus = pSpeedMultiplierBonus;
		setSpeed(_initialSpeed * _speedMultiplierBonus);
	}
}

void PongBall::resetSpeedMultiplierBonus()
{
	_speedMultiplierBonus = 1;
	setSpeed(_initialSpeed);
	_isBoosted = false;
}

void PongBall::setActive(bool isActive)
{
	if (_isActive == isActive) return;

	_isActive = isActive;

	if(_isActive)
	{
		_currentNumBounces = 0;
		setCanKill(false);
		_phantomBallEffect->begin();
	}
	else
	{
		_phantomBallEffect->stop();
		resetSpeedMultiplierBonus();
		_mainGameScene->pushInactivePongBall(this);
	}
}

bool PongBall::isActive() const
{
	return _isActive;
}

void PongBall::setCanKill(bool canKill)
{
	_canKill = canKill;

	if(_canKill)
	{
		_ballShape.setFillColor(_ballColor);

		_phantomBallEffect->setPhantomBallFillColor(_ballColor);
	}
	else
	{
		_ballShape.setFillColor(_ballInactiveColor);

		_phantomBallEffect->setPhantomBallFillColor(_ballInactiveColor);
	}
}

bool PongBall::canKill() const
{
	return _canKill;
}

void PongBall::addNumBounceAndUpdateVisibility()
{
	_currentNumBounces++;
	setCanKill(true);//Once the ball had bounced the ball can kill the player
	_mainGameScene->getAudioPlayer()->playSound("Bounce");
	
	if(_currentNumBounces > MAX_NUM_BOUNCES)
	{
		setActive(false);
	}
}

void PongBall::stopPhantomBallEffect()
{
	_phantomBallEffect->stop();
}

void PongBall::setSpeed(float pSpeed)
{
	_currentSpeed = pSpeed;
}

sf::CircleShape PongBall::getShape() const
{
	return _ballShape;
}

//--- Methods ---
void PongBall::moveEntity(const sf::Vector2f& velocity, const float& deltaTime)
{
	if (_isBoosted)
	{
		const float newSpeedRatio = Utils::deceleration(_speedMultiplierBonus, 1.f, _currentTimeBoost);
		setSpeed(newSpeedRatio * _initialSpeed);
	}

	_ballShape.move(Utils::normalize(velocity) * _currentSpeed * deltaTime);
}

//--- Boost ---
void PongBall::startBoostBall(float speedBoostBonus)
{
	_isBoosted = true;
	_currentTimeBoost = 0;

	_speedMultiplierBonus = speedBoostBonus;
	setSpeedMultiplierBonus(_speedMultiplierBonus);
}

// LINE/CIRCLE
bool PongBall::linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const
{
	sf::Vector2f outIntersectionPoint{};

	//// get length of the line
	const float lengthLine = Utils::getDistance(x1, y1, x2, y2);

	// get dot product of the line and circle
	const float dot = ((_ballShape.getPosition().x - x1)*(x2 - x1) + (_ballShape.getPosition().y - y1)*(y2 - y1)) / std::pow(lengthLine, 2);

	// find the closest point on the line
	const float closestX = x1 + (dot * (x2 - x1));
	const float closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!Utils::linePointCollision(x1, y1, x2, y2, closestX, closestY)) return false;

	// get distance to closest point
	const float distance = Utils::getDistance(closestX, closestY, _ballShape.getPosition().x, _ballShape.getPosition().y);

	if (distance <= _ballShape.getRadius())
	{
		outImpactPoint.x = closestX;
		outImpactPoint.y = closestY;
		remainingTime -= remainingTime * distance / _ballShape.getRadius();
		return true;
	}

	float ballEdgeCollTestStartX = (_velocity.x > 0 ? -_ballShape.getRadius() : _ballShape.getRadius()) * std::abs(_velocity.x);
	float ballEdgeCollTestStartY = (_velocity.y > 0 ? -_ballShape.getRadius() : _ballShape.getRadius()) * std::abs(_velocity.y);

	if (Utils::lineLineCollision(x1, y1, x2, y2, _ballShape.getPosition().x + ballEdgeCollTestStartX, _ballShape.getPosition().y + ballEdgeCollTestStartY,
		_ballDestination->getPosition().x, _ballDestination->getPosition().y, outIntersectionPoint))
	{
		Logger::Log("Traverse !! ");
		outImpactPoint = outIntersectionPoint;
		remainingTime -= remainingTime * Utils::getDistance(_ballShape.getPosition(), outImpactPoint) /
										Utils::getDistance(_ballShape.getPosition(), _ballDestination->getPosition());
		std::cout << remainingTime << std::endl;

		return true;
	}

	//if (Utils::lineLineCollision(x1, y1, x2, y2, _oldPosition.x + ballEdgeCollTestStartX, _oldPosition.y + ballEdgeCollTestStartY,
	//	_ballShape.getPosition().x + ballEdgeCollTestStartX, _ballShape.getPosition().y + ballEdgeCollTestStartY, outIntersectionPoint))
	//{
	//	Logger::Log("Traverse old position !! ");
	//	outImpactPoint = outIntersectionPoint;
	//	return true;
	//}


	return false;
}


