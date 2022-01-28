#include "SFML/Graphics.hpp"
#include "../Entity.hpp"

class Character : public Entity
{
private:
	sf::CircleShape charac;
	sf::RectangleShape canon;

	bool leftFlag = false;
	bool rightFlag = false;
	bool upFlag = false;
	bool downFlag = false;

	//Position du joueur
	int xCharDirection = 0;
	int xCanonDirection = 0;
	int yCharDirection = 0;
	int yCanonDirection = 0;

	float rotationCanon = 90.0f;
	sf::Vector2i mousePosition;
	const float SPEED = 200.0f;


public:
	Character(sf::RenderWindow& window,int xSpawn, int ySpawn, sf::Color color);
	~Character();
	void setMousePosition(sf::Vector2i mouse);

	void update(const float& deltaTime)override;
	void render(sf::RenderTarget& renderTarget)const override;
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime)override;

	// On change la rotation du personnage en fonction de la position de la souris
	void setCharacterRotation(sf::Vector2i mousePos, const float& deltaTime);


	// On change la position du bouclier en fonction de la position de la souris
	void setCanonRotation(sf::Vector2i mousePos, const float& deltaTime);


	// On défini la direction du personnage grâce aux inputs
	void direction(int isleft, int isright, int up, int down, const sf::Rect<float>& terrain, float deltaTime);

};
