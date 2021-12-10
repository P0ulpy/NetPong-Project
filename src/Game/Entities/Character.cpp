#include "Character.hpp"
#include "iostream"
#include "math.h"
#include <windows.h>

Character::Character(sf::RenderWindow& window)
{

	initParamP1(window);
	initParamP2(window);
	
}

Character::~Character()
{

}

void Character::updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window,  float clock)
{
	setCharacterPosition(xChar, yChar);
	setShieldPosition(xShie, yShie);
	
	setCharacterRotation(mousePosition(window));
	setShieldRotation(mousePosition(window));



	if (clock > 0 && clock < 1 && isActive == true)
	{
		shield.setFillColor(sf::Color::Green);
	}
	if (clock >= 1)
	{
		clock = 0;
		shield.setFillColor(sf::Color::White);
		isActive = false;
	}
	
	
	
}

void Character::updateMouvementP2(sf::RenderTarget& target, sf::RenderWindow& window,float clock)
{

	setCharacterPositionP2(xCharP2, yCharP2);
	setShieldPositionP2(xShieP2, yShieP2);

	setCharacterRotationP2(mousePosition(window));
	setShieldRotationP2(mousePosition(window));

	if (clock > 0 && clock < 1 && isActive == true)
	{
		shieldP2.setFillColor(sf::Color::Green);
	}
	if (clock >= 1)
	{
		clock = 0;
		shieldP2.setFillColor(sf::Color::White);
		isActive = false;
	}
	


}

void Character::render(sf::RenderTarget& target)
{
	target.draw(charac);
	target.draw(shield);


}

void Character::renderP2(sf::RenderTarget& target)
{

	target.draw(characP2);
	target.draw(shieldP2);

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

void Character::initParamP2(sf::RenderWindow& window) {

	characP2.setRadius(36);
	shieldP2.setSize(sf::Vector2f(70, 15));
	characP2.setFillColor(sf::Color::Cyan);
	shieldP2.setFillColor(sf::Color::White);

	sf::FloatRect characSize2 = characP2.getGlobalBounds();
	sf::FloatRect shieldSize2 = shieldP2.getGlobalBounds();

	characP2.setOrigin((characSize2.width / 2), (characSize2.height / 2));
	shieldP2.setOrigin(shieldSize2.width / 2 , (shieldSize2.height / 2) + (characSize2.height / 2));


}



void Character::setCharacterPosition( int x,int y)
{
	charac.setPosition(x, y);
}

void Character::setCharacterPositionP2(int x, int y)
{
	characP2.setPosition(x, y);
}


void Character::setShieldPosition( int x, int y)
{
	shield.setPosition(x, y);
}

void Character::setShieldPositionP2(int x, int y)
{
	shieldP2.setPosition(x, y);
}

void Character::setCharacterRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(rotation);
}

void Character::setCharacterRotationP2(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = characP2.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	characP2.setRotation(rotation);
}



void Character::setShieldRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shield.setRotation(rotation);
}

void Character::setShieldRotationP2(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = characP2.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shieldP2.setRotation(rotation);
}


void Character::direction(bool isleft, bool isright, const sf::Rect<float>& terrain)
{
	leftFlag = isleft;
	rightFlag = isright;
	if (leftFlag) xChar -= SPEED;
	if (rightFlag) xChar += SPEED;
	if (leftFlag) xShie -= SPEED;
	if (rightFlag) xShie += SPEED;

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

void Character::directionP2(bool isleft, bool isright, const sf::Rect<float>& terrain)
{
	leftFlagP2 = isleft;
	rightFlagP2 = isright;
	if (leftFlagP2) xCharP2 -= SPEED;
	if (rightFlagP2) xCharP2 += SPEED;
	if (leftFlagP2) xShieP2 -= SPEED;
	if (rightFlagP2) xShieP2 += SPEED;


	if (xCharP2 > terrain.width - (charac.getGlobalBounds().width / 2))
	{
		xCharP2 = terrain.width - (charac.getGlobalBounds().width / 2);
		xShieP2 = xCharP2;
	}

	if (xCharP2 < terrain.left + (charac.getGlobalBounds().width / 2))
	{
		xCharP2 = terrain.left + (charac.getGlobalBounds().width / 2);
		xShieP2 = xCharP2;
	}

}

sf::Vector2i Character::mousePosition(sf::RenderWindow& window) 
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	return mousePos;
}

void Character::verifActiveShield(bool clic)
{
	isActive = clic;
}