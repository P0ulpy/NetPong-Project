#include "SFML/Graphics.hpp"

#define SPEED  5
#define PI  3.14



class Character
{
private:

	
	sf::Texture sh;
	sf::Texture ch;
	//	Joueur 1
	sf::CircleShape charac;
	sf::RectangleShape shield;
	//	Joueur 2
	sf::CircleShape charac2;
	sf::RectangleShape shield2;

	bool leftFlag = false;
	bool rightFlag = false;

	bool leftFlag2 = false;
	bool rightFlag2 = false;
	float shielCooldown = 0;
	bool isActive = false;


	//Position du joueur 1 
	int xChar = 450;
	int xShie = 450;
	const int yChar = 700;
	const int yShie = 700;

	//Position du joueur 2
	int xChar2 = 450;
	int xShie2 = 450;
	const int yChar2 = 200;
	const int yShie2 = 200;


public:


	Character(sf::RenderWindow& window);
	~Character();

	void setCharacterPosition(int x,int y);
	void setCharacterPosition2(int x,int y);
	void setShieldPosition(int x,int y);
	void setShieldPosition2(int x,int y);
	void setCharacterRotation(sf::Vector2i mousePos);
	void setCharacterRotation2(sf::Vector2i mousePos);
	void setShieldRotation(sf::Vector2i mousePos);
	void setShieldRotation2(sf::Vector2i mousePos);
	void direction(bool left, bool right, const sf::Rect<float>& terrain);
	void direction2(bool left, bool right, const sf::Rect<float>& terrain);
	void initParamP1(sf::RenderWindow& window);
	void initParamP2(sf::RenderWindow& window);

	void verifActiveShield(bool isActive);
	
	void updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window, sf::Clock clock);
	void updateMouvement2(sf::RenderTarget& target, sf::RenderWindow& window);
	

	void render(sf::RenderTarget& target);
	void render2(sf::RenderTarget& target);
	
	sf::Vector2i mousePosition(sf::RenderWindow& window);



	//void update(sf::RenderTarget & target, float deltaTime);
	
	



};