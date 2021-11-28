#include "Character.hpp"
#include "iostream"
#include "math.h"

Character::Character(sf::RenderWindow& window)
{

	initParam(window);
	
}

Character::~Character()
{

}

void Character::updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window)
{

	setCharacterPosition(xChar, yChar);
	setShieldPosition(xShie, yShie);
	
	setCharacterRotation(mousePosition(window));
	setShieldRotation(mousePosition(window));
	
}

void Character::render(sf::RenderTarget& target)
{
	target.draw(charac);
	target.draw(shield);
	target.draw(hitbox);
}


void Character::initParam(sf::RenderWindow& window) {



	// Ajout des assets
	
	if (!ch.loadFromFile("./Assets/Rond_rouge.png")) {
		std::cerr << "Error while loading texture" << std::endl;
	}
	if (!sh.loadFromFile("./Assets/Bouclier.jpg")) {
		std::cerr << "Error while loading bouclier texture" << std::endl;
	}
	
	//création d'une hitbox
	



	//Préparation des sprites

	//largeur charac = 477
	//largeur shield = 223
	//hauteur shield = 51

	charac.setRadius(36);
	shield.setSize(sf::Vector2f(70,15));

	charac.setFillColor(sf::Color::Red);

	shield.setFillColor(sf::Color::Cyan);




	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect shieldSize = shield.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height/ 2));
	shield.setOrigin(shieldSize.width /2, (shieldSize.height /2)+ (characSize.height  /2));
	
	
}



void Character::setCharacterPosition( int x,int y)
{

	charac.setPosition(x, y);
	

}

void Character::setShieldPosition( int x, int y)
{
	//sf::Vector2f p = charac.getTransform().transformPoint(sf::Vector2f(charac.getOrigin().x -( (charac.getGlobalBounds().width / charac.getScale().y) / 2), charac.getOrigin().y));
	//float px = charac.getTransform().transformPoint(sf::Vector2f(charac.getOrigin().x, (charac.getOrigin().y - (charac.getGlobalBounds().width / charac.getScale().y) / 2))).x;
	//float py = charac.getTransform().transformPoint(sf::Vector2f(charac.getOrigin().x , (charac.getOrigin().y - (charac.getGlobalBounds().width / charac.getScale().y) / 2))).y;
	

	
	
	shield.setPosition(x, y);
	hitbox.setPosition(x, y);
//	std::cout << charac.getTransform().transformPoint(1,2).x << std::endl;

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
	//if (xChar > terrain.width - (shield.getGlobalBounds().width / 2)) xShie = terrain.width - (shield.getGlobalBounds().width / 2);
	//if (xShie < terrain.left + (shield.getGlobalBounds().width / 2)) xShie = terrain.left + (shield.getGlobalBounds().width / 2);

}

sf::Vector2i Character::mousePosition(sf::RenderWindow& window) {

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	return mousePos;
}





