#include "PongBall.hpp"

#include <iostream>

#include "PhantomBall.hpp"
#include "../PolygonTerrain.hpp"

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
	_velocity = sf::Vector2f(0.8f, 3.f);
	_initialSpeed = 220.f;
	_currentSpeed = _initialSpeed;
	_maxSpeed = 2000.f;

	_texture = std::make_unique<sf::Texture>();
	_sprite = std::make_unique<sf::Sprite>();

	_ballSize = BALL_SIZE;
	_ballColor = sf::Color(0, 200, 0, 255);

	_speedMultiplierBonus = 1.f;
}

void PongBall::initShapes(const sf::RenderWindow& window)
{
	_ballShape.setRadius(static_cast<float>(_ballSize));
	_ballShape.setFillColor(_ballColor);
	_ballShape.setOutlineColor(sf::Color::White);
	_ballShape.setOutlineThickness(2);
	_ballShape.setOrigin(_ballShape.getGlobalBounds().width / 2.f, _ballShape.getGlobalBounds().height / 2.f);
	_ballShape.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f));
}

void PongBall::initBoost()
{
	_isBoosted = false;
	_boostDuration = 2.f;
	_currentTimeBoost = 0.f;
}

void PongBall::initPhantomEffect()
{
	_phantomBallsMax = 10;
	_durationBetweenPhantomBalls = 0.05f;

	createPhantomBalls();

	startPhantomBallEffect();
}

//--- Updating ---
void PongBall::update(const float& deltaTime)
{
	updateBoost(deltaTime);
	updateCollision();
	updateMovement(deltaTime);
	updatePhantomEffect(deltaTime);
}

void PongBall::updateCollision()
{
	const int pointCount = static_cast<int>(_polygonTerrain->getShape().getPointCount());

	sf::Vector2f outImpactPoint{ 0,0 };

	for (int i = 0; i < pointCount; i++)
	{
		const float xA = _polygonTerrain->getPointPosition(i).x;
		const float yA = _polygonTerrain->getPointPosition(i).y;
		const float xB = _polygonTerrain->getPointPosition((i + 1) % pointCount).x;
		const float yB = _polygonTerrain->getPointPosition((i + 1) % pointCount).y;

		bool hit = lineCircleCollision(xA, yA, xB, yB, _ballShape.getPosition().x, _ballShape.getPosition().y, _ballShape.getRadius(), outImpactPoint);

		if (hit)
		{
			const sf::Vector2f surfaceVector = normalize(sf::Vector2f(xB - xA, yB - yA));
			_velocity = _polygonTerrain->getVectorReflection(_velocity, surfaceVector);

			const auto normalSurfaceVector = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
			_ballShape.setPosition(outImpactPoint.x + normalSurfaceVector.x * _ballShape.getRadius() * 1.1f,
				outImpactPoint.y + normalSurfaceVector.y * _ballShape.getRadius() * 1.1f);
		}
	}
}

void PongBall::updateMovement(const float& deltaTime)
{
	moveEntity(_velocity, deltaTime);
}

void PongBall::updateBoost(const float& deltaTime)
{
	if (_isBoosted)
	{
		_currentTimeBoost += deltaTime;
		if (_currentTimeBoost >= _boostDuration)
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

		if (_currentTimePhantomBallCooldown > _durationBetweenPhantomBalls)
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
	target.draw(_ballShape);
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
		const float newSpeedRatio = deceleration(_speedMultiplierBonus, 1.f, _currentTimeBoost);
		setSpeed(newSpeedRatio * _initialSpeed);
	}

	_ballShape.move(normalize(velocity) * _currentSpeed * deltaTime);
}

//--- Boost ---
void PongBall::startBoostBall(float speedBoostBonus)
{
	_isBoosted = true;
	_currentTimeBoost = 0;

	_speedMultiplierBonus = speedBoostBonus;
	setSpeedMultiplierBonus(_speedMultiplierBonus);
}

float PongBall::deceleration(const float initial, const float target, const float time) const
{
	//Linear(Y0,Y1,t) = Y0 + t(Y1 - Y0)
	//Deceleration(Y0,Y1,t) = Linear( Y0, Y1, 1 - pow(1 - t,2) )
	//t = 1 - pow(1 - t,2), donc :
	//LinearDec(Y0,Y1,t) = Y0 + ( 1 - pow(1 - t,2) ) * (Y1 - Y0) )
	return initial + (1 - std::pow(1 - time / 2, 2)) * (target - initial);
}

sf::Vector2f PongBall::normalize(const sf::Vector2f& originalVector)
{
	const float norm = std::sqrt((originalVector.x * originalVector.x) + (originalVector.y * originalVector.y));

	// Prevent division by zero
	if (norm <= std::numeric_limits<float>::epsilon() * norm * 2 //2 -> constexpr units_in_last_place
		|| norm < std::numeric_limits<float>::min())
	{
		return sf::Vector2f{};
	}

	return originalVector / norm;
}

float PongBall::dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

//--- Phantom balls effect ---
void PongBall::createPhantomBalls()
{
	for (int i = 0; i < _phantomBallsMax; i++)
	{
		_phantomBalls.push_back(std::make_unique<PhantomBall>(*this));
	}
}

void PongBall::displayPhantomBall()
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
bool PongBall::lineCircleCollision(float x1, float y1, float x2, float y2, float cx, float cy, float r, sf::Vector2f& outImpactPoint) const
{
	// get length of the line
	const float lengthLine = getDistance(x1, y1, x2, y2);

	// get dot product of the line and circle
	const float dot = (((cx - x1)*(x2 - x1)) + ((cy - y1)*(y2 - y1))) / std::pow(lengthLine, 2);

	// find the closest point on the line
	const float closestX = x1 + (dot * (x2 - x1));
	const float closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!linePointCollision(x1, y1, x2, y2, closestX, closestY)) return false;

	// get distance to closest point
	const float distance = getDistance(closestX, closestY, cx, cy);

	if (distance <= r) {
		outImpactPoint.x = closestX;
		outImpactPoint.y = closestY;
		return true;
	}

	return false;
}

// LINE/POINT
bool PongBall::linePointCollision(float x1, float y1, float x2, float y2, float px, float py) const
{
	// get distance from the point to the two ends of the line
	const float d1 = getDistance(px, py, x1, y1);
	const float d2 = getDistance(px, py, x2, y2);

	// get the length of the line
	const float lineLen = getDistance(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	const float buffer = 0.1f;    // higher # = less accurate

	// if the two distances are equal to the line's 
	// length, the point is on the line!
	// note we use the buffer here to give a range, 
	// rather than one #
	return d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer;
}

// POINT/CIRCLE
bool PongBall::pointCircleCollision(float px, float py, float cx, float cy, float r) const
{

	// get distance between the point and circle's center
	// using the Pythagorean Theorem

	// if the distance is less than the circle's
	// radius the point is inside!
	return getDistance(px, py, cx, cy) <= r;
}

float PongBall::getDistance (float x1, float y1, float x2, float y2) const
{
	const float distX = x1 - x2;
	const float distY = y1 - y2;
	return std::sqrt(distX*distX + distY*distY);
}


