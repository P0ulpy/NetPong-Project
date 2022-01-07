#include "PongBall.hpp"

#include <iostream>

#include "PhantomBall.hpp"
#include "../../Utils/Utils.hpp"
#include "../Terrains/PolygonTerrain.hpp"

constexpr int BALL_SIZE = 20;

constexpr int MAX_NUM_BOUNCES = 2;
constexpr float BOOST_DURATION = 2.f;

constexpr int MAX_PHANTOM_BALLS = 20;
constexpr float DURATION_BETWEEN_PHANTOM_BALLS = .025f;


//--- Constructors - Destructor ---
PongBall::PongBall(const sf::RenderWindow& window, const sf::Rect<float>& terrain, PolygonTerrain& polyTerrain)
	: _terrainArea(terrain), _polygonTerrain(&polyTerrain)
{
	initVariables();
	initShapes(window);
	initBoost();
	initPhantomEffect();
}

PongBall::~PongBall()
{

}

//--- Initializers ---
void PongBall::initVariables()
{
	setActive(true);

	_velocity = Utils::normalize(sf::Vector2f(1.f, 2.f));
	_initialSpeed = 220.f;
	_currentSpeed = _initialSpeed;
	_maxSpeed = 10000.f;

	_texture = std::make_unique<sf::Texture>();
	_sprite = std::make_unique<sf::Sprite>();

	_ballSize = BALL_SIZE;
	_ballColor = sf::Color(65, 90, 255, 255);

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

void PongBall::initPhantomEffect()
{
	createPhantomBalls();

	startPhantomBallEffect();
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
	if(!_ballShape.getGlobalBounds().intersects(sf::Rect<float>(0,0,1200,900)))
	{
		std::cout << "Out of window !" << std::endl;
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
	if(_hasPhantomEffect)
	{
		_currentTimePhantomBallCooldown += deltaTime;

		if (_currentTimePhantomBallCooldown > DURATION_BETWEEN_PHANTOM_BALLS)
		{
			displayPhantomBall();
			_currentTimePhantomBallCooldown = 0;
		}
	}

	for (const auto& phantomBall : _phantomBalls)
	{
		if (phantomBall->isDisplayed())
		{
			phantomBall->update(deltaTime);
		}
	}
}

//--- Rendering ---
void PongBall::render(sf::RenderTarget& target) const
{
	renderPhantomEffect(target);

	if(_isActive)
	{
		target.draw(_ballShape);
		target.draw(*_ballDestination);
	}
}

void PongBall::renderPhantomEffect(sf::RenderTarget& target) const
{
	for (const auto& phantomBall : _phantomBalls)
	{
		if (phantomBall->isDisplayed())
		{
			phantomBall->render(target);
		}
	}
}

void PongBall::updateAndRenderPhantomEffect(sf::RenderTarget& target, const float& deltaTime)
{
	for (const auto& phantomBall : _phantomBalls)
	{
		if (phantomBall->isDisplayed())
		{
			phantomBall->update(deltaTime);
			phantomBall->render(target);
		}
	}
}

bool PongBall::hitWallIfCollision(float x1, float y1, float x2, float y2)
{
	sf::Vector2f outImpactPoint{ 0,0 };

	bool hit = linePongBallCollision(x1, y1, x2, y2, outImpactPoint);

	if (hit)
	{
		const sf::Vector2f surfaceVector = Utils::normalize(sf::Vector2f(x2 - x1, y2 - y1));
		_velocity = Utils::getVectorReflection(_velocity, surfaceVector);

		const auto normalSurfaceVector = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
		_ballShape.setPosition(outImpactPoint.x + normalSurfaceVector.x * _ballShape.getRadius(),
			outImpactPoint.y + normalSurfaceVector.y * _ballShape.getRadius());

		addNumBounceAndUpdateVisibility();

		return true;
	}

	return false;
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

void PongBall::addSpeedMultiplierBonus(float pSpeedMultiplierBonus)
{
	if ((_speedMultiplierBonus + pSpeedMultiplierBonus) * _currentSpeed < _maxSpeed)
	{
		_speedMultiplierBonus += pSpeedMultiplierBonus;
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
	_isActive = isActive;

	if(_isActive)
	{
		_currentNumBounces = 0;
		startPhantomBallEffect();
	}
	else
	{
		stopPhantomBallEffect();
		resetSpeedMultiplierBonus();
	}
}

bool PongBall::isActive() const
{
	return _isActive;
}

void PongBall::addNumBounceAndUpdateVisibility()
{
	_currentNumBounces++;

	if(_currentNumBounces > MAX_NUM_BOUNCES)
	{
		setActive(false);
	}
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

//--- Phantom balls effect ---
void PongBall::createPhantomBalls()
{
	for (int i = 0; i < MAX_PHANTOM_BALLS; i++)
	{
		_phantomBalls.push_back(std::make_unique<PhantomBall>(*this));
	}
}

void PongBall::displayPhantomBall()
{
	bool isPhantomBallDisplayed = false;
	//Recherche de la premiere PhantomBall qui n'est pas affich�e dans la liste
	for (const auto& phantomBall : _phantomBalls)
	{
		if (!phantomBall->isDisplayed())
		{
			phantomBall->show();
			isPhantomBallDisplayed = true;
			break;
		}
	}

	if(!isPhantomBallDisplayed)
	{
		std::cout << "ERROR PongBall.cpp | displayPhantomBall() : pas assez de _phantomBallsMax ! " << std::endl;
	}
}

void PongBall::startPhantomBallEffect()
{
	_hasPhantomEffect = true;
	_currentTimePhantomBallCooldown = 0;
}

void PongBall::stopPhantomBallEffect()
{
	_hasPhantomEffect = false;
}

// LINE/CIRCLE
bool PongBall::linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint) const
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
		return true;
	}

	float ballEdgeCollTestStartX = (_velocity.x > 0 ? -_ballShape.getRadius() : _ballShape.getRadius()) * std::abs(_velocity.x);
	float ballEdgeCollTestStartY = (_velocity.y > 0 ? -_ballShape.getRadius() : _ballShape.getRadius()) * std::abs(_velocity.y);

	if (Utils::lineLineCollision(x1, y1, x2, y2, _ballShape.getPosition().x + ballEdgeCollTestStartX, _ballShape.getPosition().y + ballEdgeCollTestStartY,
		_ballDestination->getPosition().x, _ballDestination->getPosition().y, outIntersectionPoint))
	{
		std::cout << "Traverse !! " << std::endl;
		outImpactPoint = outIntersectionPoint;
		return true;
	}

	if (Utils::lineLineCollision(x1, y1, x2, y2, _oldPosition.x + ballEdgeCollTestStartX, _oldPosition.y + ballEdgeCollTestStartY,
		_ballShape.getPosition().x + ballEdgeCollTestStartX, _ballShape.getPosition().y + ballEdgeCollTestStartY, outIntersectionPoint))
	{
		std::cout << "Traverse old position !! " << std::endl;
		outImpactPoint = outIntersectionPoint;
		return true;
	}


	return false;
}


