#include "Character.hpp"
#include "iostream"
#include <math.h>    
#include <windows.h>
#include "string"
#include "../../Utils/Utils.hpp"
#include "../Entities/PongBall.hpp"

constexpr float CHARAC_SPEED = 300.f;

constexpr float DURATION_BETWEEN_SHOOTS = 0.3;
constexpr float COOLDOWN_RELOAD_FIRST_BALL = 0.75f;
constexpr float COOLDOWN_RELOAD_SECOND_BALL = COOLDOWN_RELOAD_FIRST_BALL * 2;

constexpr float CHARAC_GLOBAL_SIZE = 1.5f;
constexpr int CHARAC_RADIUS = 18 * CHARAC_GLOBAL_SIZE;
constexpr int AMMO_SIDE_SIZE = 10 * CHARAC_GLOBAL_SIZE;
constexpr int SHOOT_ZONE_RADIUS = 5 * CHARAC_GLOBAL_SIZE;
constexpr int CANON_SIZE_X = 25 * CHARAC_GLOBAL_SIZE;
constexpr int CANON_SIZE_Y = 15 * CHARAC_GLOBAL_SIZE;

constexpr int DEFAULT_SPAWN_POS_X = 500;
constexpr int DEFAULT_SPAWN_POS_Y = 500;

Character::Character(sf::Color color)
{
	//Pr�paration du personnage
	charac.setRadius(CHARAC_RADIUS);
	charac.setFillColor(color);

	//Pr�paration du canon
	canon.setSize(sf::Vector2f(CANON_SIZE_X, CANON_SIZE_Y));
	canon.setFillColor(sf::Color::White);

	//Pr�paration de la zone d'o� la balle part
	shootZone.setRadius(SHOOT_ZONE_RADIUS);
	shootZone.setFillColor(sf::Color::Magenta);

	//Premi�re munition
	firstAmmo.setSize(sf::Vector2f(AMMO_SIDE_SIZE, AMMO_SIDE_SIZE));
	firstAmmo.setFillColor(sf::Color::White);

	//Deuxi�me munition
	secondAmmo.setSize(sf::Vector2f(AMMO_SIDE_SIZE, AMMO_SIDE_SIZE));
	secondAmmo.setFillColor(sf::Color::White);

	setPosition(DEFAULT_SPAWN_POS_X, DEFAULT_SPAWN_POS_Y);

	//Modification des origines
	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect canonSize = canon.getGlobalBounds();
	sf::FloatRect ammoSize = firstAmmo.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height / 2));
	canon.setOrigin(canonSize.width / 2 + (characSize.height / 2), (canonSize.height / 2));
	shootZone.setOrigin(canonSize.width, 0);
	firstAmmo.setOrigin( -(characSize.width/2) - ammoSize.width/2,ammoSize.height/2 + ammoSize.height);
	secondAmmo.setOrigin( -(characSize.width / 2) - ammoSize.width/2,ammoSize.height/2 - ammoSize.height);

	//Test for collision
	_characDestination = new sf::CircleShape();
	_characDestination->setRadius(5.f);
	_characDestination->setFillColor(sf::Color::Red);
	_characDestination->setOrigin(_characDestination->getGlobalBounds().width / 2.f, _characDestination->getGlobalBounds().height / 2.f);
	_characDestination->setPosition(charac.getPosition());
}

Character::~Character()
{ }

void Character::setMousePosition(sf::Vector2i mouse)
{
	mousePosition = mouse;
}


void Character::setPosition(int xSpawn, int ySpawn)
{
	charac.setPosition(xSpawn, ySpawn);
	canon.setPosition(xSpawn, ySpawn);
	shootZone.setPosition(xSpawn, ySpawn);
	firstAmmo.setPosition(xSpawn, ySpawn);
	secondAmmo.setPosition(xSpawn, ySpawn);
}

void Character::toggleCharacterMove(bool canCharacterMove)
{
	_canCharacterMove = canCharacterMove;
}

bool Character::canCharacterMove() const
{
	return _canCharacterMove;
}

void Character::update(const float& deltaTime)
{
	setRotation(mousePosition);
	if (_cooldownActivated)
	{
		if (_cooldownShoot >= 0 && _cooldownShoot < DURATION_BETWEEN_SHOOTS) _cooldownShoot = _cooldownShoot + deltaTime;

		if (_cooldownShoot > DURATION_BETWEEN_SHOOTS)
		{
			activateCooldown(false);
			_cooldownShoot = 0;
		}

	}

	if (_isReloading)
	{
		if (_reloadingTime >= 0 && _reloadingTime < COOLDOWN_RELOAD_FIRST_BALL)
		{
			_reloadingTime = _reloadingTime + deltaTime;
		}
		if (_reloadingTime >= COOLDOWN_RELOAD_FIRST_BALL && _reloadingTime < COOLDOWN_RELOAD_SECOND_BALL)
		{
			_reloadingTime = _reloadingTime + deltaTime;
			_ammos = 1;
		}
		if (_reloadingTime > COOLDOWN_RELOAD_SECOND_BALL)
		{
			activateReloading(false);
			_reloadingTime = 0;
			_ammos = 2;
		}
	}

	_characDestination->setPosition(
		std::abs(charac.getPosition().x) + Utils::normalize(_velocity).x * _currentSpeed * deltaTime,
		std::abs(charac.getPosition().y) + Utils::normalize(_velocity).y * _currentSpeed * deltaTime
	);
}

void Character::moveEntity(const sf::Vector2f& direction, const float& deltaTime) 
{
	charac.move(Utils::normalize(direction) * CHARAC_SPEED * deltaTime);
	canon.move(Utils::normalize(direction) * CHARAC_SPEED * deltaTime);
	shootZone.move(Utils::normalize(direction) * CHARAC_SPEED * deltaTime);
	firstAmmo.move(Utils::normalize(direction) * CHARAC_SPEED * deltaTime);
	secondAmmo.move(Utils::normalize(direction) * CHARAC_SPEED * deltaTime);
}

void Character::render(sf::RenderTarget& target)const
{
	if (_isAlive)
	{
		target.draw(canon);
		target.draw(charac);
		if (_ammos > 1)
		{
			target.draw(firstAmmo);
			target.draw(secondAmmo);
		}
		else if (_ammos > 0)
		{
			target.draw(secondAmmo);
		}
	}
}

void Character::setRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = canon.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	_rotation = ((atan2(dy, dx)) * 180.0 / PI);
	canon.setRotation(_rotation);
	shootZone.setRotation(_rotation);
	firstAmmo.setRotation(_rotation);
	secondAmmo.setRotation(_rotation);
}

void Character::direction(int isleft, int isright, int up, int down, float deltaTime)
{
	if (!canCharacterMove()) return;
	xCharDirection = -isleft + isright;
	yCharDirection = -up + down;

	_velocity = sf::Vector2f(xCharDirection, yCharDirection);
	moveEntity(_velocity, deltaTime);
}

bool Character::hitWallIfCollision(float x1, float y1, float x2, float y2)
{
	sf::Vector2f outImpactPoint{ 0,0 };

	bool hit = characterCollision(x1, y1, x2, y2, outImpactPoint);

	if (hit)
	{
		const sf::Vector2f surfaceVector = Utils::normalize(sf::Vector2f(x2 - x1, y2 - y1));
		const auto normalSurfaceVector = sf::Vector2f(-surfaceVector.y, surfaceVector.x);

		_velocity = Utils::getVectorReflection(_velocity, surfaceVector);

		charac.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x,
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y);

		canon.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x,
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y);

		shootZone.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x,
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y);

		firstAmmo.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x,
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y);

		secondAmmo.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x,
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y);

		return true;
	}

	return false;
}

bool Character::characterCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint) const
{
	sf::Vector2f outIntersectionPoint{};

	//// get length of the line
	const float lengthLine = Utils::getDistance(x1, y1, x2, y2);

	// get dot product of the line and circle
	const float dot = ((charac.getPosition().x - x1) * (x2 - x1) + (charac.getPosition().y - y1) * (y2 - y1)) / std::pow(lengthLine, 2);

	// find the closest point on the line
	const float closestX = x1 + (dot * (x2 - x1));
	const float closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!Utils::linePointCollision(x1, y1, x2, y2, closestX, closestY)) return false;

	// get distance to closest point
	const float distance = Utils::getDistance(closestX, closestY, charac.getPosition().x, charac.getPosition().y);

	if (distance <= charac.getRadius())
	{
		outImpactPoint.x = closestX;
		outImpactPoint.y = closestY;
		return true;
	}

	//Continous collision detection
	float ballEdgeCollTestStartX = (_velocity.x > 0 ? -charac.getRadius() : charac.getRadius()) * std::abs(_velocity.x);
	float ballEdgeCollTestStartY = (_velocity.y > 0 ? -charac.getRadius() : charac.getRadius()) * std::abs(_velocity.y);

	if (Utils::lineLineCollision(x1, y1, x2, y2, charac.getPosition().x + ballEdgeCollTestStartX, charac.getPosition().y + ballEdgeCollTestStartY,
		_characDestination->getPosition().x, _characDestination->getPosition().y, outIntersectionPoint))
	{
		outImpactPoint = outIntersectionPoint;
		Utils::getDistance(charac.getPosition(), _characDestination->getPosition());

		return true;
	}

	return false;
}

void Character::ammoCount(int ammo)
{
	_ammos = _ammos + ammo;
}

sf::Vector2f Character::shootDirection(sf::Vector2i mousePos)
{
	float radianDirection = _rotation * PI / 180;
	return sf::Vector2f(-(float)cos(radianDirection), -(float)sin(radianDirection));
}

sf::Vector2f Character::shootDepart()
{
	return sf::Vector2f(shootZone.getTransform().transformPoint(0, 0).x, shootZone.getTransform().transformPoint(0, 0).y);
}

void Character::activateCooldown(bool activate)
{
	if (_ammos == 0)activateReloading(true);
	else _cooldownActivated = activate;
}
void Character::activateReloading(bool activateReload)
{
	_isReloading = activateReload;
}

void Character::setAmmosColor(sf::Color normalColor, sf::Color inactiveColor)
{
	_ammoColorNormal = normalColor;
	_ammoColorInactive = inactiveColor;
}

sf::Color Character::getInactiveAmmoColor() const
{
	return _ammoColorInactive;
}

sf::Color Character::getNormalAmmoColor() const
{
	return _ammoColorNormal;
}

bool Character::isInCooldown()
{
	return _cooldownActivated;
}

bool Character::isReloading()
{
	return _isReloading;
}

//x = position en x, y = position en y et z = rayon 
sf::Vector2f Character::getPosition()
{
	return sf::Vector2f(charac.getPosition().x, charac.getPosition().y);
}

float Character::getRadius()
{
	return charac.getGlobalBounds().width / 2;
}

void Character::setPlayerAlive(bool isAlive)
{
	_isAlive = isAlive;
}



