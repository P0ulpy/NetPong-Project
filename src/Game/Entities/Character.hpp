#include "SFML/Graphics.hpp"

#define SPEED  5
#define PI  3.14



class Character
{
private:

	
	sf::Texture sh;
	sf::Texture ch;
	sf::Sprite shield;
	sf::Sprite charac;
	bool leftFlag = false;
	bool rightFlag = false;

	int xChar = 450;
	int xShie = 450;
	const int yChar = 700;
	const int yShie = 700;


public:


	Character(sf::RenderWindow& window);
	~Character();

	void setCharacterPosition(int xChar,int yChar);
	void setShieldPosition(int xShie,int yShie);
	void setCharacterRotation(sf::Vector2i mousePos);
	void setShieldRotation(sf::Vector2i mousePos);
	void direction(bool left, bool right, const sf::Rect<float>& terrain);
	void initParam(sf::RenderWindow& window);

	void updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window);
	

	void render(sf::RenderTarget& target);
	
	sf::Vector2i mousePosition(sf::RenderWindow& window);


	//void update(sf::RenderTarget & target, float deltaTime);
	
	



};