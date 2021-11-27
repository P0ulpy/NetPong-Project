#include "PongBall.hpp"

#include <iostream>

#include "PhantomBall.hpp"
#include "../PolygonTerrain.hpp"
#include "../PolygonCollisionResult.hpp"

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
	_velocity = sf::Vector2f(1.f, 1.f);
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
	_ballShape.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f));

	_ballHitboxShape.setFillColor(sf::Color(0,0,0,0));
	_ballHitboxShape.setOutlineColor(sf::Color::Red);
	_ballHitboxShape.setOutlineThickness(2);
	_ballHitboxShape.setPosition(_ballShape.getPosition());
}

void PongBall::initBoost()
{
	_isBoosted = false;
	_boostDuration = 2.f;
	_currentTimeBoost = 0.f;
}

void PongBall::initPhantomEffect()
{
	_phantomBallsMax = 15;
	_durationBetweenPhantomBalls = 0.05f;

	createPhantomBalls();

	startPhantomBallEffect();
}

//--- Updating ---
void PongBall::update(const float& deltaTime)
{
	updateBoost(deltaTime);
	updateMovement(deltaTime);
	updatePhantomEffect(deltaTime);
}

void PongBall::updateMovement(const float& deltaTime)
{
	if(_hasHit)
	{
		_currentTimeStopCollisionTests += deltaTime;

		if (_currentTimeStopCollisionTests > _durationStopCollisionTests)
		{
			_hasHit = false;
			_currentTimeStopCollisionTests = 0;
		}
		else
		{
			return;
		}
	}

	for(int i  = 0 ; i < _polygonTerrain->getShape().getPointCount() ; i++)
	{
		if(i != _polygonTerrain->getShape().getPointCount()-1)
		{
			float xA = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(i).x, _polygonTerrain->getShape().getPoint(i).y)).x;
			float yA = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(i).x, _polygonTerrain->getShape().getPoint(i).y)).y;
			float xB = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(i + 1).x, _polygonTerrain->getShape().getPoint(i + 1).y)).x;
			float yB = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(i + 1).x, _polygonTerrain->getShape().getPoint(i + 1).y)).y;

			sf::Vector2f circleCenter = sf::Vector2f(_ballShape.getGlobalBounds().left + _ballShape.getGlobalBounds().width / 2.f,
				_ballShape.getGlobalBounds().top + _ballShape.getGlobalBounds().height / 2.f);

			bool hit = lineCircleCollision(xA, yA, xB, yB, circleCenter.x, circleCenter.y, _ballShape.getRadius());

			if (hit)
			{
				const sf::Vector2f surfaceVector = (sf::Vector2f(xB - xA, yB - yA));

				_velocity = _polygonTerrain->getVectorReflection(_velocity, normalize(surfaceVector));
				_hasHit = true;
				break;
			}
		}
		else
		{
			float xA = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(_polygonTerrain->getShape().getPointCount() - 1).x, _polygonTerrain->getShape().getPoint(_polygonTerrain->getShape().getPointCount() - 1).y)).x;
			float yA = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(_polygonTerrain->getShape().getPointCount() - 1).x, _polygonTerrain->getShape().getPoint(_polygonTerrain->getShape().getPointCount() - 1).y)).y;
			float xB = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(0).x, _polygonTerrain->getShape().getPoint(0).y)).x;
			float yB = _polygonTerrain->getShape().getTransform().transformPoint(sf::Vector2f(_polygonTerrain->getShape().getPoint(0).x, _polygonTerrain->getShape().getPoint(0).y)).y;

			sf::Vector2f circleCenter = sf::Vector2f(_ballShape.getGlobalBounds().left + _ballShape.getGlobalBounds().width / 2.f,
				_ballShape.getGlobalBounds().top + _ballShape.getGlobalBounds().height / 2.f);

			bool hit = lineCircleCollision(xA, yA, xB, yB, circleCenter.x, circleCenter.y, _ballShape.getRadius());

			if (hit)
			{
				const sf::Vector2f surfaceVector = (sf::Vector2f(xB - xA, yB - yA));

				_velocity = _polygonTerrain->getVectorReflection(_velocity, normalize(surfaceVector));
				_hasHit = true;
				break;
			}
		}
	}

	moveEntity(_velocity, deltaTime);
	_ballHitboxShape.setPosition(_ballShape.getPosition());
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
	target.draw(_ballHitboxShape);
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
	//Recherche de la premiere PhantomBall qui n'est pas affichée dans la liste
	for (const auto& phantomBall : _phantomBalls)
	{
		if (!phantomBall->isDisplayed())
		{
			phantomBall->show();
			break;
		}
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
bool PongBall::lineCircleCollision(float x1, float y1, float x2, float y2, float cx, float cy, float r) {

	// is either end INSIDE the circle?
	// if so, return true immediately
	bool inside1 = pointCircleCollision(x1, y1, cx, cy, r);
	bool inside2 = pointCircleCollision(x2, y2, cx, cy, r);
	if (inside1 || inside2) return true;

	// get length of the line
	float len = getDistance(x1, y1, x2, y2);

	// get dot product of the line and circle
	float dot = (((cx - x1)*(x2 - x1)) + ((cy - y1)*(y2 - y1))) / std::pow(len, 2);

	// find the closest point on the line
	float closestX = x1 + (dot * (x2 - x1));
	float closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	bool onSegment = linePointCollision(x1, y1, x2, y2, closestX, closestY);
	if (!onSegment) return false;

	// get distance to closest point
	float distance = getDistance(closestX, closestY, cx, cy);

	if (distance <= r) {
		return true;
	}
	return false;
}

// LINE/POINT
bool PongBall::linePointCollision(float x1, float y1, float x2, float y2, float px, float py) {

	// get distance from the point to the two ends of the line
	float d1 = getDistance(px, py, x1, y1);
	float d2 = getDistance(px, py, x2, y2);

	// get the length of the line
	float lineLen = getDistance(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	float buffer = 10.f;    // higher # = less accurate

	// if the two distances are equal to the line's 
	// length, the point is on the line!
	// note we use the buffer here to give a range, 
	// rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		return true;
	}
	return false;
}

// POINT/CIRCLE
bool PongBall::pointCircleCollision(float px, float py, float cx, float cy, float r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	float distance = getDistance(px, py, cx, cy);

	// if the distance is less than the circle's
	// radius the point is inside!
	if (distance <= r) {
		return true;
	}
	return false;
}

float PongBall::getDistance (float x1, float y1, float x2, float y2)
{
	float distX = x1 - x2;
	float distY = y1 - y2;
	return std::sqrt(distX*distX + distY*distY);
}


