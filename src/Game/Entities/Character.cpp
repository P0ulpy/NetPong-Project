#include "Character.hpp"
#include "math.h"

constexpr int SPEED = 250;

Character::Character(sf::RenderWindow& window,  int xSpawn, int ySpawn,sf::Color color)
{
	xChar = xSpawn;
	xShie = xSpawn;
	yChar = ySpawn;
	yShie = ySpawn;
	_currentSpeed = SPEED;
	charac.setPosition(xChar, yChar);
	shield.setPosition(xChar, yChar);
	charac.setRadius(36);
	shield.setSize(sf::Vector2f(70, 15));
	charac.setFillColor(color);
	shield.setFillColor(sf::Color::White);

	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect shieldSize = shield.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height / 2));
	shield.setOrigin(shieldSize.width / 2, (shieldSize.height / 2) + (characSize.height / 2));

	
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
	_velocity = sf::Vector2f(xChar, yChar);
	moveEntity(_velocity, deltaTime);
	_velocity = sf::Vector2f(xShie, yShie);
	moveEntity(_velocity, deltaTime);


	setCharacterRotation(mousePosition);
	setShieldRotation(mousePosition);

}



void Character::moveEntity(const sf::Vector2f& position, const float& deltaTime) 
{
	//std::cout << "D�placement : " << position.y << std::endl;
	charac.setPosition(position);
	//characP2.move(velocity * _currentSpeed * deltaTime);
	shield.setPosition(position);
	//shieldP2.move(velocity * _currentSpeed * deltaTime);

	
}






void Character::render(sf::RenderTarget& target)const
{
	target.draw(charac);
	target.draw(shield);
}





void Character::initParam(sf::RenderWindow& window) {

	charac.setRadius(36);
	shield.setSize(sf::Vector2f(70,15));
	charac.setFillColor(sf::Color::Red);
	shield.setFillColor(sf::Color::White);

	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect shieldSize = shield.getGlobalBounds();

	charac.setOrigin((characSize.width / 2), (characSize.height/ 2));
	shield.setOrigin(shieldSize.width /2, (shieldSize.height /2)+ (characSize.height  /2));
	
	
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
	//std::cout << "Gauche : " << leftFlag << std::endl;
	//std::cout << "Droite : " << rightFlag << std::endl;
	//std::cout << "Haut : " << upFlag << std::endl;
	//std::cout << "Bas : " << downFlag << std::endl;
	//std::cout<<std::endl;

	if (leftFlag) {
		xChar -= (SPEED * deltaTime);
		xShie -= (SPEED * deltaTime);
	}
	if (rightFlag) {
		xChar = xChar + (SPEED * deltaTime);
		xShie = xChar + (SPEED * deltaTime);
	}
	if (upFlag) {
		yChar -= (SPEED * deltaTime);
		yShie -= (SPEED * deltaTime);
	}
	if (downFlag) {
		yChar += (SPEED * deltaTime);
		yShie += (SPEED * deltaTime);
	}


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