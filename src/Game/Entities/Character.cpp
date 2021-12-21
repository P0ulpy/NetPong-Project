#include "Character.hpp"
#include "iostream"
#include "math.h"
#include <windows.h>
constexpr int SPEED = 250;
Character::Character(sf::RenderWindow& window, sf::Vector2f spawnPoint)
{
	
	tel parametre




	_currentSpeed = SPEED;

	initParamP1(window);
	initParamP2(window);
	
}

Character::~Character()
{

}

void Character::update(const float& deltaTime) 
{
	updateMouvement()

}

void Character::moveEntity(const sf::Vector2f& velocity, const float& deltaTime) 
{
	charac.move(velocity * _currentSpeed * deltaTime);
	characP2.move(velocity * _currentSpeed * deltaTime);
	shield.move(velocity * _currentSpeed * deltaTime);
	shieldP2.move(velocity * _currentSpeed * deltaTime);
}
void Character::updateMouvement()
{
	setCharacterPosition(xChar, yChar);
	setShieldPosition(xShie, yShie);

	setCharacterRotation(mousePosition(window));
	setShieldRotation(mousePosition(window));

}





void Character::render(sf::RenderTarget& target)const
{
	target.draw(charac);
	target.draw(shield);
}





void Character::initParamP1(sf::RenderWindow& window) {

	charac.setRadius(36);
	shield.setSize(sf::Vector2f(70,15));
	charac.setFillColor(sf::Color::Red);
	shield.setFillColor(sf::Color::White);

	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect shieldSize = shield.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height/ 2));
	shield.setOrigin(shieldSize.width /2, (shieldSize.height /2)+ (characSize.height  /2));
	
	
}
/*
void Character::initParamP2(sf::RenderWindow& window) {

	characP2.setRadius(36);
	shieldP2.setSize(sf::Vector2f(70, 15));
	characP2.setFillColor(sf::Color::Cyan);
	shieldP2.setFillColor(sf::Color::White);

	sf::FloatRect characSize2 = characP2.getGlobalBounds();
	sf::FloatRect shieldSize2 = shieldP2.getGlobalBounds();

	characP2.setOrigin((characSize2.width / 2), (characSize2.height / 2));
	shieldP2.setOrigin(shieldSize2.width / 2 , (shieldSize2.height / 2) + (characSize2.height / 2));


}*/



void Character::setCharacterPosition( int x,int y)
{
	charac.setPosition(x, y);
}




void Character::setShieldPosition( int x, int y)
{
	shield.setPosition(x, y);
}

void Character::setCharacterRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(rotation);
}





void Character::setShieldRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shield.setRotation(rotation);
}




void Character::direction(bool isleft, bool isright, bool up, bool down, const sf::Rect<float>& terrain, float deltaTime)
{
	leftFlag = isleft;
	rightFlag = isright;
	upFlag = up;
	downFlag = down;

	if (leftFlag) xChar -= (SPEED * deltaTime);
	if (rightFlag) xChar += (SPEED * deltaTime);

	if (leftFlag) xShie -= (SPEED * deltaTime);
	if (rightFlag) xShie += (SPEED * deltaTime);

	if (upFlag) yChar -= (SPEED * deltaTime);
	if (downFlag) yChar += (SPEED * deltaTime);

	if (upFlag) yShie -= (SPEED * deltaTime);
	if (downFlag) yShie += (SPEED * deltaTime);

	if (xChar > terrain.width - (charac.getGlobalBounds().width / 2 ))
	{
		xChar = terrain.width - (charac.getGlobalBounds().width / 2);
		xShie = xChar;
	}

	if (xChar < terrain.left + (charac.getGlobalBounds().width / 2))
	{
		xChar = terrain.left + (charac.getGlobalBounds().width / 2);
		xShie = xChar;
	}



}



void Character::verifActiveShield(bool clic)
{
	isActive = clic;
}