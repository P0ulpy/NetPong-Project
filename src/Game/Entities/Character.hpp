#include "SFML/Graphics.hpp"
#include "../Entity.hpp"

//class PongBall;

class Character : public Entity
{
private:
	sf::CircleShape charac;
	sf::RectangleShape canon;
	sf::CircleShape shootZone;

	sf::RectangleShape firstAmmo;
	sf::RectangleShape secondAmmo;


	//Direction du joueur
	int xCharDirection = 0;
	int yCharDirection = 0;
	//Rotation du joueur
	float _rotation;
	float _cooldownShoot = 0;

	bool _cooldownActivated = false;
	bool _isReloading = false;

	float _reloadingTime = 0;

	sf::Vector2i mousePosition;
	const float SPEED = 200.0f;

	int _ammos = 2;



public:
	Character(sf::RenderWindow& window,int xSpawn, int ySpawn, sf::Color color);
	~Character();

	void setMousePosition(sf::Vector2i mouse);

	void update(const float& deltaTime)override;
	void render(sf::RenderTarget& renderTarget)const override;
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime)override;

	// set la rotation des sprites en fonction de la souris
	void setRotation(sf::Vector2i mousePos);
	bool isInCooldown();
	bool isReloading();

	void activateCooldown(bool activate);
	void activateReloading(bool activateReload);


	// On d�fini la direction du personnage gr�ce aux inputs
	void direction(int isleft, int isright, int up, int down, const sf::Rect<float>& terrain, float deltaTime);

	bool hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime);
	bool characterCollisionWall(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const;

	sf::Vector2f shootDirection(sf::Vector2i mousePos);
	sf::Vector2f shootDepart();

	void ammoCount(int ammo);


};
