#include "Character.hpp"
#include "iostream"
#include "math.h"

Character::Character()
{
	
	


	if (!ch.loadFromFile("./Assets/Rond_rouge.png")) {
		std::cerr << "Error while loading texture" << std::endl;
	}
	if (!sh.loadFromFile("./Assets/Barre.jpg")) {
		std::cerr << "Error while loading bouclier texture" << std::endl;
	}

	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);


	charac.setTexture(ch);
	shield.setTexture(sh);

	charac.setScale(0.2,0.2);
	shield.setScale(0.1,0.1);

	sf::FloatRect characSize = charac.getGlobalBounds();
	sf::FloatRect shieldSize = shield.getGlobalBounds();

	charac.setOrigin((characSize.width / 2) / charac.getScale().x, (characSize.height / 2) / charac.getScale().y);
	shield.setOrigin((shieldSize.width / 2) / shield.getScale().x, (shieldSize.height / 2) / shield.getScale().y);

	int xChar = window.getSize().x / 2.;
	int yChar = window.getSize().y / 2.;
	int xShie = window.getSize().x / 1.2;
	int yShie = window.getSize().y / 1.2;

	bool leftFlag = false;
	bool rightFlag = false;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "JAPPUIE LA" << std::endl;

				switch (event.key.code)
				{

					case sf::Keyboard::Escape: window.close(); break;
					case sf::Keyboard::Left: leftFlag = true; break;
					case sf::Keyboard::Right: rightFlag = true; break;
					default:break;

				}

				

			}

			if (event.type == sf::Event::KeyReleased)
			{

				switch (event.key.code)
				{

				case sf::Keyboard::Left: leftFlag = false; break;
				case sf::Keyboard::Right: rightFlag = false; break;
				default:break;

				}



			}
		}
	}

	

	if (leftFlag) xChar -= SPEED;
	if (rightFlag) xChar += SPEED;
	if (leftFlag) xShie -= SPEED;
	if (rightFlag) xShie += SPEED;


	if (xChar < 0) xChar = 0;
	if (xChar > (int)window.getSize().x) xChar = window.getSize().x;
	if (xShie < 0) xShie = 0;
	if (xShie > (int)window.getSize().x) xShie = window.getSize().x;



	setCharacterPosition(xChar,yChar);
	setShieldPosition(xShie,yShie);
	setCharacterRotation();
	setShieldRotation();

	window.draw(charac);
	window.draw(shield);



}

Character::~Character()
{

}






void Character::setCharacterPosition( int x,int y)
{
	
	charac.setPosition(x, y);


}

void Character::setShieldPosition( int x, int y)
{
	sf::Vector2f p = charac.getTransform().transformPoint(sf::Vector2f(charac.getOrigin().x, charac.getOrigin().y - (charac.getGlobalBounds().width) * 2.5));
	shield.setPosition(p.x, p.y);

}

void Character::setCharacterRotation()
{
	sf::Vector2f curPos = charac.getPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	charac.setRotation(rotation);


}



void Character::setShieldRotation()
{
	sf::Vector2f curPos = charac.getPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float dx = curPos.x - mousePos.x;
	float dy = curPos.y - mousePos.y;

	float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;
	shield.setRotation(rotation);


}

