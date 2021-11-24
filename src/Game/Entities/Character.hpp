#include "SFML/Graphics.hpp"

#define SPEED  5
#define PI  3.14



class Character
{
private:
	sf::RenderWindow window;
	sf::Texture sh;
	sf::Texture ch;
	sf::Sprite shield;
	sf::Sprite charac;
public:


	Character();
	~Character();

	void setCharacterPosition(int xChar,int yChar);
	void setShieldPosition(int xShie,int yShie);
	void setCharacterRotation();
	void setShieldRotation();
	
	
	



};