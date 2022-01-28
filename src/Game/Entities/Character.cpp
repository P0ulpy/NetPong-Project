#include "Character.hpp"
#include "iostream"
#include "math.h"
#include <windows.h>
#include "string"
#include "../../Utils/Utils.hpp"


Character::Character(sf::RenderWindow& window,  int xSpawn, int ySpawn,sf::Color color)
{
	charac.setPosition(xSpawn, ySpawn);
	canon.setPosition(xSpawn, ySpawn);
	charac.setRadius(36);
	canon.setSize(sf::Vector2f(50, 20));
	charac.setFillColor(color);
	canon.setFillColor(sf::Color::White);
	

	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect canonSize = canon.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height / 2));
	canon.setOrigin(canonSize.width / 2 + (characSize.height / 2), (canonSize.height / 2));

	

	
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
	setCharacterRotation(mousePosition,deltaTime);
	setCanonRotation(mousePosition,deltaTime);
}



void Character::moveEntity(const sf::Vector2f& direction, const float& deltaTime) 
{
	charac.move(Utils::normalize(direction) * SPEED * deltaTime);
	canon.move(Utils::normalize(direction) * SPEED * deltaTime);
}






void Character::render(sf::RenderTarget& target)const
{
	target.draw(canon);
	target.draw(charac);
	
}




void Character::setCharacterRotation(sf::Vector2i mousePos ,const float& deltaTime)
{
	sf::Vector2f curPos = charac.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(rotation);
}





void Character::setCanonRotation(sf::Vector2i mousePos, const float& deltaTime)
{
	sf::Vector2f curPos = canon.getPosition();
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI);
	canon.setRotation(rotation);
}




void Character::direction(int isleft, int isright, int up, int down, const sf::Rect<float>& terrain, float deltaTime)
{
	

	xCharDirection = -isleft + isright;
	xCanonDirection = -isleft + isright;
	yCharDirection = -up + down;
	yCanonDirection = -up + down;

	_velocity = sf::Vector2f(xCharDirection, yCharDirection);
	moveEntity(_velocity, deltaTime);
	_velocity = sf::Vector2f(xCanonDirection, yCanonDirection);
	moveEntity(_velocity, deltaTime);

	
	//Si les coordonnées en x du milieu du personnage sont supérieurs a la largeur du terrain on stop le personnage contre le mur  
	
	
	if (charac.getPosition().x > terrain.width - (charac.getGlobalBounds().width / 2))
	{
		charac.setPosition(terrain.width - (charac.getGlobalBounds().width / 2),charac.getPosition().y);
		canon.setPosition(charac.getPosition().x, canon.getPosition().y);
	}
	// En gros : Si les coordonnées en x du milieu du personnage sont inférieurs a la gauche du terrain on stop le personnage contre le mur  
	if (charac.getPosition().x < terrain.left + (charac.getGlobalBounds().width / 2))
	{
		charac.setPosition(terrain.left + (charac.getGlobalBounds().width / 2), charac.getPosition().y);
		canon.setPosition(charac.getPosition().x, canon.getPosition().y);
	}
	
	if (charac.getPosition().y < terrain.top - (charac.getGlobalBounds().width / 2))
	{
		charac.setPosition( charac.getPosition().x, terrain.top - (charac.getGlobalBounds().width / 2));
		canon.setPosition( canon.getPosition().x, charac.getPosition().y);
	}
	
	if (charac.getPosition().y > terrain.height + (charac.getGlobalBounds().width / 2))
	{
		charac.setPosition(charac.getPosition().x, terrain.height + (charac.getGlobalBounds().width / 2));
		canon.setPosition(canon.getPosition().x, charac.getPosition().y);
	}


}
