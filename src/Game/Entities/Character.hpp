#include "SFML/Graphics.hpp"
#include "../Entity.hpp"

//class PongBall;

class Character : public Entity
{
private:
	sf::CircleShape charac;
	sf::RectangleShape canon;
	sf::CircleShape shootZone;

	bool leftFlag = false;
	bool rightFlag = false;
	bool upFlag = false;
	bool downFlag = false;

	//Position du joueur
	int xCharDirection = 0;
	int xCanonDirection = 0;
	int yCharDirection = 0;
	int yCanonDirection = 0;

	float _rotationCharac;
	float _rotationCanon;
	float _rotationShootZone;

	
	sf::Vector2i mousePosition;
	const float SPEED = 200.0f;

	//const std::vector<PongBall*>& _pongBalls;


public:
	Character(sf::RenderWindow& window,int xSpawn, int ySpawn, sf::Color color);
	~Character();
	void setMousePosition(sf::Vector2i mouse);

	void update(const float& deltaTime)override;
	void render(sf::RenderTarget& renderTarget)const override;
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime)override;

	// On change la rotation du personnage en fonction de la position de la souris
	void setCharacterRotation(sf::Vector2i mousePos, const float& deltaTime);
	void setCanonRotation(sf::Vector2i mousePos, const float& deltaTime);
	void setShootZoneRotation(sf::Vector2i mousePos, const float& deltaTime);

	// On d�fini la direction du personnage gr�ce aux inputs
	void direction(int isleft, int isright, int up, int down, const sf::Rect<float>& terrain, float deltaTime);

	bool hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime);
	bool linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const;

	void shoot();
	sf::Vector2f shootDirection(sf::Vector2i mousePos);
	sf::Vector2f shootDepart();


};
