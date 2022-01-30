#include "Character.hpp"
#include "iostream"
#include <math.h>    
#include <windows.h>
#include "string"
#include "../../Utils/Utils.hpp"
#include "../Entities/PongBall.hpp"

Character::Character(sf::RenderWindow& window,  int xSpawn, int ySpawn,sf::Color color)
{
	//Préparation du personnage
	charac.setPosition(xSpawn, ySpawn);
	charac.setRadius(36);
	charac.setFillColor(color);
	//Préparation du canon
	canon.setPosition(xSpawn, ySpawn);
	canon.setSize(sf::Vector2f(50, 30));
	canon.setFillColor(sf::Color::White);
	//Préparation de la zone d'où la balle part
	shootZone.setRadius(10);
	shootZone.setPosition(xSpawn,ySpawn);
	shootZone.setFillColor(sf::Color::Magenta);
	//Préparation des indicateurs de munitions
	//Première munition
	firstAmmo.setPosition(xSpawn, ySpawn);
	firstAmmo.setSize(sf::Vector2f(20, 20));
	firstAmmo.setFillColor(sf::Color::White);
	//Deuxième munition
	secondAmmo.setPosition(xSpawn, ySpawn);
	secondAmmo.setSize(sf::Vector2f(20, 20));
	secondAmmo.setFillColor(sf::Color::White);


	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect canonSize = canon.getGlobalBounds();
	sf::FloatRect ammoSize = firstAmmo.getGlobalBounds();


	charac.setOrigin((characSize.width / 2), (characSize.height / 2));
	canon.setOrigin(canonSize.width / 2 + (characSize.height / 2), (canonSize.height / 2));
	shootZone.setOrigin(canonSize.width, 0);
	firstAmmo.setOrigin( -(characSize.width/2) - ammoSize.width/2,ammoSize.height/2 + ammoSize.height);
	secondAmmo.setOrigin( -(characSize.width / 2) - ammoSize.width/2,ammoSize.height/2 - ammoSize.height);

}

Character::~Character()
{

}

void Character::setMousePosition(sf::Vector2i mouse)
{
	mousePosition = mouse;
}


void Character::update(const float& deltaTime)
{
	setRotation(mousePosition);
	if (_cooldownActivated)
	{
		if (_cooldownShoot >= 0 && _cooldownShoot < 1)_cooldownShoot = _cooldownShoot + deltaTime;

		if (_cooldownShoot > 1)
		{
			activateCooldown(false);
			_cooldownShoot = 0;
		}

	}

	if (_isReloading)
	{
		if (_reloadingTime >= 0 && _reloadingTime < 1.5) 
		{
			_reloadingTime = _reloadingTime + deltaTime;
		}
		if (_reloadingTime >= 1.5 && _reloadingTime < 3)
		{
			_reloadingTime = _reloadingTime + deltaTime;
			_ammos = 1;
		}
		if (_reloadingTime > 3)
		{
			activateReloading(false);
			_reloadingTime = 0;
			_ammos = 2;
		}

	}
	
}

void Character::moveEntity(const sf::Vector2f& direction, const float& deltaTime) 
{
	charac.move(Utils::normalize(direction) * SPEED * deltaTime);
	canon.move(Utils::normalize(direction) * SPEED * deltaTime);
	shootZone.move(Utils::normalize(direction) * SPEED * deltaTime);
	firstAmmo.move(Utils::normalize(direction) * SPEED * deltaTime);
	secondAmmo.move(Utils::normalize(direction) * SPEED * deltaTime);
}



void Character::render(sf::RenderTarget& target)const
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




void Character::direction(int isleft, int isright, int up, int down, const sf::Rect<float>& terrain, float deltaTime)
{
	xCharDirection = -isleft + isright;
	yCharDirection = -up + down;

	_velocity = sf::Vector2f(xCharDirection, yCharDirection);
	moveEntity(_velocity, deltaTime);
}

bool Character::hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime)
{
	sf::Vector2f outImpactPoint{ 0,0 };

	bool hit = characterCollisionWall(x1, y1, x2, y2, outImpactPoint, remainingTime);

	if (hit)
	{
		const sf::Vector2f surfaceVector = Utils::normalize(sf::Vector2f(x2 - x1, y2 - y1));
		const auto normalSurfaceVector = sf::Vector2f(-surfaceVector.y, surfaceVector.x);

		_velocity = Utils::getVectorReflection(_velocity, surfaceVector);

		charac.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x * (remainingTime),
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y * (remainingTime));

		canon.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x * (remainingTime),
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y * (remainingTime));

		shootZone.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x * (remainingTime),
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y * (remainingTime));

		firstAmmo.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x * (remainingTime),
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y * (remainingTime));

		secondAmmo.setPosition(outImpactPoint.x + normalSurfaceVector.x * charac.getRadius() + _velocity.x * (remainingTime),
			outImpactPoint.y + normalSurfaceVector.y * charac.getRadius() + _velocity.y * (remainingTime));



		return true;
	}

	return false;
}

bool Character::characterCollisionWall(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const
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
		remainingTime -= remainingTime * distance / charac.getRadius();
		return true;
	}

	return false;
}



void Character::ammoCount(int ammo)
{
	_ammos = _ammos + ammo;
	std::cout << "Ammos = " << _ammos << std::endl;
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

bool Character::isInCooldown()
{
	return _cooldownActivated;
}

bool Character::isReloading()
{
	return _isReloading;
}


