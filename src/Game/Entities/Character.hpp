#include "SFML/Graphics.hpp"
#include "../Entity.hpp"

#define PI  3.14

class Character : public Entity
{
private:

	
	sf::Texture sh;
	sf::Texture ch;
	//	Joueur 1
	sf::CircleShape charac;
	sf::RectangleShape shield;


	bool leftFlag = false;
	bool rightFlag = false;
	bool upFlag = false;
	bool downFlag = false;


	bool isActive = false;


	//Position du joueur 1 
	int xChar = 450;
	int xShie = 450;

	int yChar = 700;
	int yShie = 700;

	

public:



	Character(sf::RenderWindow& window);
	~Character();

	void update(const float& deltaTime)override;
	void render(sf::RenderTarget& renderTarget)const override;
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime)override;

	// On change la position du personnage en fonction de la speed et sa direction
	void setCharacterPosition(int x,int y);


	// On change la position du bouclier en fonction de la speed et de la direction du personnage
	void setShieldPosition(int x,int y);


	// On change la rotation du personnage en fonction de la position de la souris
	void setCharacterRotation(sf::Vector2i mousePos);


	// On change la position du bouclier en fonction de la position de la souris
	void setShieldRotation(sf::Vector2i mousePos);


	// On défini la direction du personnage grâce aux inputs
	void direction(bool left, bool right,bool up, bool down, const sf::Rect<float>& terrain, float deltaTime);


	// On défini les paramètres des deux personnages
	void initParamP1(sf::RenderWindow& window);

	
	// On vérifie si le bouclier est actif ou non
	void verifActiveShield(bool isActive);
	

	// Update des personnage
	void updateMouvement( sf::RenderWindow& window,float clock);

	
	

	// Affichage des personnages
	

	
	//sf::Vector2i mousePosition(sf::RenderWindow& window);

};