#include "PongBall.h"

//--- Constructors - Destructor ---
PongBall::PongBall(const sf::RenderWindow& window, const sf::Rect<float>& terrain)
	: _terrainArea(terrain)
{
	initVariables();
	initBoost();
	initShape(window);
	initPhantomEffect();
}

PongBall::~PongBall()
{

}

//--- Initializers ---
void PongBall::initVariables()
{
	_ballSize = 20;
	_ballColor = sf::Color(0, 200, 0, 255);

	_velocity = sf::Vector2f(0.75f, 1.f);
	_initialSpeed = 220.f;
	_currentSpeed = _initialSpeed;
	_maxSpeed = 2000.f;
	_speedBonusRatio = 1.f;
}

void PongBall::initShape(const sf::RenderWindow& window)
{
	_ballShape.setRadius(static_cast<float>(_ballSize));
	_ballShape.setFillColor(_ballColor);
	_ballShape.setOutlineColor(sf::Color::White);
	_ballShape.setOutlineThickness(2);
	_ballShape.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
}

void PongBall::initBoost()
{
	_boostDuration = 2.f;
	_currentTimeBoost = 0.f;
}

void PongBall::initPhantomEffect()
{
	_phantomBallsMax = 15;

	_timeBetweenPhantomBalls = 0.05f;
	_phantomBallEffectDuration = 1.f;
	_hasPhantomEffect = false;

	_currentTimePhantomBallEffect = 0;
	_currentTimePhantomBallCooldown = 0;

	createPhantomBalls();
}

//--- Updating ---
void PongBall::update(sf::RenderTarget& target, const float deltaTime)
{
	updateMovement(deltaTime);
	updatePhantomEffect(deltaTime);
}

void PongBall::updateMovement(const float& deltaTime)
{
	//LEFT
	if (_ballShape.getGlobalBounds().left + 1 <= _terrainArea.left)
	{
		_velocity.x = -_velocity.x;
		_ballShape.setPosition(_terrainArea.left, _ballShape.getGlobalBounds().top);
	}

	//RIGHT
	if (_ballShape.getGlobalBounds().left + _ballShape.getGlobalBounds().width >= _terrainArea.width)
	{
		_velocity.x = -_velocity.x;
		_ballShape.setPosition(_terrainArea.width - _ballShape.getGlobalBounds().width, _ballShape.getGlobalBounds().top);
	}

	//TOP
	if (_ballShape.getGlobalBounds().top + 1 <= _terrainArea.top)
	{
		_velocity.y = -_velocity.y;
		_ballShape.setPosition(_ballShape.getGlobalBounds().left, _terrainArea.top);
	}

	//BOTTOM
	if (_ballShape.getGlobalBounds().top + _ballShape.getGlobalBounds().height >= _terrainArea.height)
	{
		_velocity.y = -_velocity.y;
		_ballShape.setPosition(_ballShape.getGlobalBounds().left, _terrainArea.height - _ballShape.getGlobalBounds().height);
	}

	//Update boost
	if(_isBoosted)
	{
		_currentTimeBoost += deltaTime;
		if(_currentTimeBoost >= _boostDuration)
		{
			_isBoosted = false;
			resetSpeedBonusRatio();
		}
	}

	moveBall(deltaTime);
}

void PongBall::updatePhantomEffect(const float& deltaTime)
{
	/*if (_needErasePhantomBall)
	{
		eraseLastPhantomBall();
	}*/

	if(_hasPhantomEffect)
	{
		_currentTimePhantomBallCooldown += deltaTime;
		_currentTimePhantomBallEffect += deltaTime;

		if (_currentTimePhantomBallCooldown > _timeBetweenPhantomBalls)
		{
			displayPhantomBall();
			_currentTimePhantomBallCooldown = 0;
		}

		if(_currentTimePhantomBallEffect > _phantomBallEffectDuration)
		{
			//_hasPhantomEffect = false;
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
		if(phantomBall->isDisplayed())
		{
			phantomBall->render(target);
		}
	}
}

//--- Getters - Setters ---
void PongBall::setTerrainArea(const sf::Rect<float>& terrain)
{
	_terrainArea = terrain;
}

float PongBall::getCurrentBoostTimeStamp() const
{
	return (_currentTimeBoost / _boostDuration) * 2;
}

void PongBall::setSpeedBonusRatio(float pSpeedBonusRatio)
{
	if (pSpeedBonusRatio * _currentSpeed < _maxSpeed)
	{
		_speedBonusRatio = pSpeedBonusRatio;
		setSpeed(_initialSpeed *_speedBonusRatio);
	}
}

void PongBall::addSpeedBonusRatio(float pSpeedBonusRatio)
{
	if ((_speedBonusRatio + pSpeedBonusRatio) * _currentSpeed < _maxSpeed)
	{
		_speedBonusRatio += pSpeedBonusRatio;
		setSpeed(_initialSpeed *_speedBonusRatio);
	}
}

void PongBall::resetSpeedBonusRatio()
{
	_speedBonusRatio = 1;
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
void PongBall::moveBall(const float& deltaTime)
{
	if(_isBoosted)
	{
		const float newSpeedRatio = deceleration(_speedBonusRatio, 1.f, _currentTimeBoost);
		setSpeed(newSpeedRatio * _initialSpeed);
	}

	_ballShape.move(normalize(_velocity) * _currentSpeed * deltaTime);
}

//--- Boost ---
void PongBall::boostBall(float speedBoost)
{
	_currentTimeBoost = 0;
	_isBoosted = true;
	_speedBonusRatio = speedBoost;

	setSpeedBonusRatio(_speedBonusRatio);
	startPhantomBallEffect();
}

float PongBall::deceleration(float initial, float target, float time) const
{
	//Linear(Y0,Y1,t) = Y0 + t(Y1 - Y0)
	//Deceleration(Y0,Y1,t) = Linear( Y0, Y1, 1 - pow(1 - t,2) )
	//t = 1 - pow(1 - t,2), donc :
	//LinearDec(Y0,Y1,t) = Y0 + ( 1 - pow(1 - t,2) ) * (Y1 - Y0) )
	return initial + (1 - std::pow(1 - time / 2, 2)) * (target - initial);
}

sf::Vector2f PongBall::normalize(const sf::Vector2f& originalVector) const
{
	const float norm = std::sqrt((originalVector.x * originalVector.x) + (originalVector.y * originalVector.y));

	// Prevent division by zero
	if (norm <= std::numeric_limits<float>::epsilon() * norm * 2 //2 -> units_in_last_place
		|| norm < std::numeric_limits<float>::min())
	{
		return sf::Vector2f{};
	}

	return originalVector / norm;
}

//--- Phantom balls effect ---
void PongBall::startPhantomBallEffect()
{
	_currentTimePhantomBallEffect = 0;
	_currentTimePhantomBallCooldown = 0;
	_hasPhantomEffect = true;
}

void PongBall::createPhantomBalls()
{
	for(int i = 0 ; i < _phantomBallsMax ; i++)
	{
		_phantomBalls.push_back(std::make_unique<PhantomBall>(*this));
	}
}

void PongBall::displayPhantomBall()
{
	for (const auto& phantomBall : _phantomBalls)
	{
		if (!phantomBall->isDisplayed())
		{
			phantomBall->displayPhantomBall();
			break;
		}
	}
}

void PongBall::activatedErasingLastPhantomBall()
{
	_needErasePhantomBall = true;
}

void PongBall::eraseLastPhantomBall()
{
	_needErasePhantomBall = false;
	if (!_phantomBalls.empty())
	{
		_phantomBalls.erase(_phantomBalls.begin());
	}
}


