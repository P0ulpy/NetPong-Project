#include "Character.hpp"
#include "iostream"
#include <math.h>    
#include <windows.h>
#include "string"
#include "../../Utils/Utils.hpp"
#include "../Entities/PongBall.hpp"

Character::Character(sf::RenderWindow& window,  int xSpawn, int ySpawn,sf::Color color)
{
	charac.setPosition(xSpawn, ySpawn);
	charac.setRadius(36);
	charac.setFillColor(color);

	canon.setPosition(xSpawn, ySpawn);
	canon.setSize(sf::Vector2f(50, 30));
	canon.setFillColor(sf::Color::White);

	shootZone.setRadius(10);
	shootZone.setPosition(xSpawn,ySpawn);
	shootZone.setFillColor(sf::Color::Magenta);



	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect canonSize = canon.getGlobalBounds();
	sf::FloatRect shootZoneSize = shootZone.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height / 2));
	canon.setOrigin(canonSize.width / 2 + (characSize.height / 2), (canonSize.height / 2));
	shootZone.setOrigin(canonSize.width, 0);
	


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
	
	setCharacterRotation(mousePosition, deltaTime);
	setCanonRotation(mousePosition,deltaTime);
	setShootZoneRotation(mousePosition, deltaTime);
	
}

void Character::moveEntity(const sf::Vector2f& direction, const float& deltaTime) 
{
	charac.move(Utils::normalize(direction) * SPEED * deltaTime);
	canon.move(Utils::normalize(direction) * SPEED * deltaTime);
	shootZone.move(Utils::normalize(direction) * SPEED * deltaTime);
}

void Character::render(sf::RenderTarget& target)const
{
	target.draw(canon);
	target.draw(charac);
	//target.draw(shootZone);
	
}

void Character::setCharacterRotation(sf::Vector2i mousePos ,const float& deltaTime)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	_rotationCharac = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(_rotationCharac);
}

void Character::setShootZoneRotation(sf::Vector2i mousePos, const float& deltaTime)
{
	sf::Vector2f curPos = shootZone.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	_rotationShootZone = ((atan2(dy, dx)) * 180.0 / PI) ;
	shootZone.setRotation(_rotationShootZone);
}

void Character::setCanonRotation(sf::Vector2i mousePos, const float& deltaTime)
{
	sf::Vector2f curPos = canon.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	_rotationCanon = ((atan2(dy, dx)) * 180.0 / PI);
	canon.setRotation(_rotationCanon);
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

	bool hit = linePongBallCollision(x1, y1, x2, y2, outImpactPoint, remainingTime);

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



		return true;
	}

	return false;
}

bool Character::linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const
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

void Character::shoot() 
{

}

sf::Vector2f Character::shootDirection(sf::Vector2i mousePos)
{
	float radianDirection = _rotationShootZone * PI / 180;
	return sf::Vector2f(-(float)cos(radianDirection), -(float)sin(radianDirection));
}

sf::Vector2f Character::shootDepart()
{
	return sf::Vector2f(shootZone.getTransform().transformPoint(0, 0).x, shootZone.getTransform().transformPoint(0, 0).y);
}
