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
	sf::CircleShape characP2;
	sf::RectangleShape shieldP2;

	bool leftFlag = false;
	bool rightFlag = false;

	bool leftFlagP2 = false;
	bool rightFlagP2 = false;

	bool isActive = false;


	//Position du joueur 1 
	int xChar = 450;
	int xShie = 450;
	const int yChar = 700;
	const int yShie = 700;

	//Position du joueur 2
	int xCharP2 = 450;
	int xShieP2 = 450;
	const int yCharP2 = 200;
	const int yShieP2 = 200;


public:


	Character(sf::RenderWindow& window);
	~Character();

	// On change la position du personnage en fonction de la speed et sa direction
	void setCharacterPosition(int x,int y);
	void setCharacterPositionP2(int x,int y);

	// On change la position du bouclier en fonction de la speed et de la direction du personnage
	void setShieldPosition(int x,int y);
	void setShieldPositionP2(int x,int y);

	// On change la rotation du personnage en fonction de la position de la souris
	void setCharacterRotation(sf::Vector2i mousePos);
	void setCharacterRotationP2(sf::Vector2i mousePos);

	// On change la position du bouclier en fonction de la position de la souris
	void setShieldRotation(sf::Vector2i mousePos);
	void setShieldRotationP2(sf::Vector2i mousePos);

	// On défini la direction du personnage grâce aux inputs
	void direction(bool left, bool right, const sf::Rect<float>& terrain);
	void directionP2(bool left, bool right, const sf::Rect<float>& terrain);

	// On défini les paramètres des deux personnages
	void initParamP1(sf::RenderWindow& window);
	void initParamP2(sf::RenderWindow& window);
	
	// On vérifie si le bouclier est actif ou non
	void verifActiveShield(bool isActive);
	

	// Update des personnage
	void updateMouvement(sf::RenderTarget& target, sf::RenderWindow& window,float clock);
	void updateMouvementP2(sf::RenderTarget& target, sf::RenderWindow& window,float clock);
	
	// Affichage des personnages
	void render(sf::RenderTarget& target);
	void renderP2(sf::RenderTarget& target);
	
	sf::Vector2i mousePosition(sf::RenderWindow& window);

};