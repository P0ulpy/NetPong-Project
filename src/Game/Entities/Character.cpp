#include "Character.hpp"
#include "iostream"
#include "math.h"
#include <windows.h>
#include <chrono>
#include <time.h>
#include <string>  

Character::Character(sf::RenderWindow& window)
{

	initParamP1(window);
	initParamP2(window);
	
}

Character::~Character()
{

}

void Character::updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window,  sf::Clock clock)
{

	setCharacterPosition(xChar, yChar);
	setShieldPosition(xShie, yShie);
	
	setCharacterRotation(mousePosition(window));
	setShieldRotation(mousePosition(window));

	sf::Time elapsed = clock.getElapsedTime();
	sf::Time avantUtilisation = clock.getElapsedTime();
	//std::cout << isActive << std::endl;
	shielCooldown = elapsed.asSeconds();

	
	
	if (shielCooldown > 0 && shielCooldown < 1 && isActive == true)
	{
		shield.setFillColor(sf::Color::Green);
	}
	if (shielCooldown >= 1)
	{
		shielCooldown = 0;
		shield.setFillColor(sf::Color::White);
		isActive = false;
	}
	
	
	
}

void Character::updateMouvement2(sf::RenderTarget& target, sf::RenderWindow& window)
{

	setCharacterPosition2(xChar2, yChar2);
	setShieldPosition2(xShie2, yShie2);

	setCharacterRotation2(mousePosition(window));
	setShieldRotation2(mousePosition(window));

	


}

void Character::render(sf::RenderTarget& target)
{
	target.draw(charac);
	target.draw(shield);


}

void Character::render2(sf::RenderTarget& target)
{

	target.draw(charac2);
	target.draw(shield2);

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

	charac2.setRadius(36);
	shield2.setSize(sf::Vector2f(70, 15));
	charac2.setFillColor(sf::Color::Cyan);
	shield2.setFillColor(sf::Color::White);

	sf::FloatRect characSize2 = charac2.getGlobalBounds();
	sf::FloatRect shieldSize2 = shield2.getGlobalBounds();

	charac2.setOrigin((characSize2.width / 2), (characSize2.height / 2));
	shield2.setOrigin(shieldSize2.width / 2 , (shieldSize2.height / 2) + (characSize2.height / 2));


}



void Character::setCharacterPosition( int x,int y)
{

	charac.setPosition(x, y);

	
}

void Character::setCharacterPosition2(int x, int y)
{

	charac2.setPosition(x, y);

}


void Character::setShieldPosition( int x, int y)
{

	shield.setPosition(x, y);


}

void Character::setShieldPosition2(int x, int y)
{

	shield2.setPosition(x, y);


}

void Character::setCharacterRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(rotation);


}

void Character::setCharacterRotation2(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac2.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac2.setRotation(rotation);


}



void Character::setShieldRotation(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shield.setRotation(rotation);



}

void Character::setShieldRotation2(sf::Vector2i mousePos)
{
	sf::Vector2f curPos = charac2.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shield2.setRotation(rotation);


}


void Character::direction(bool isleft, bool isright, const sf::Rect<float>& terrain)
{
	
	leftFlag = isleft;
	rightFlag = isright;
	if (leftFlag) xChar -= SPEED;
	if (rightFlag) xChar += SPEED;
	if (leftFlag) xShie -= SPEED;
	if (rightFlag) xShie += SPEED;
	
	
	if (xChar > terrain.width - (charac.getGlobalBounds().width / 2)) 
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

void Character::direction2(bool isleft, bool isright, const sf::Rect<float>& terrain)
{

	leftFlag2 = isleft;
	rightFlag2 = isright;
	if (leftFlag2) xChar2 -= SPEED;
	if (rightFlag2) xChar2 += SPEED;
	if (leftFlag2) xShie2 -= SPEED;
	if (rightFlag2) xShie2 += SPEED;


	if (xChar2 > terrain.width - (charac.getGlobalBounds().width / 2))
	{
		xChar2 = terrain.width - (charac.getGlobalBounds().width / 2);
		xShie2 = xChar2;
	}

	if (xChar2 < terrain.left + (charac.getGlobalBounds().width / 2))
	{
		xChar2 = terrain.left + (charac.getGlobalBounds().width / 2);
		xShie2 = xChar2;
	}


}

sf::Vector2i Character::mousePosition(sf::RenderWindow& window) {

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	return mousePos;
}

void Character::verifActiveShield(bool clic)
{
	isActive = clic;
}