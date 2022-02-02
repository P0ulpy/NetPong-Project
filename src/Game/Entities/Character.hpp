#include "SFML/Graphics.hpp"
#include "../Entity.hpp"

//class PongBall;

class Character : public Entity
{
private:
	sf::CircleShape charac;
	sf::RectangleShape canon;
	sf::CircleShape shootZone;

	//For collision testing purposes
	sf::CircleShape* _characDestination;

	//Ammos
	sf::RectangleShape firstAmmo;
	sf::RectangleShape secondAmmo;



	//PongBall colors
	sf::Color _ammoColorNormal;
	sf::Color _ammoColorInactive;

	//Direction du joueur
	int xCharDirection = 0;
	int yCharDirection = 0;
	//Rotation du joueur
	float _rotation;
	//Cadence de tir
	bool _cooldownActivated = false;
	float _cooldownShoot = 0;
	float _endCooldown = 1;

	//Rechargement des balles
	bool _isReloading = false;
	float _reloadingTime = 0;
	float _endReloading = 3;

	sf::Vector2i mousePosition;

	//Nombre de balles max
	int _ammos = 2;

public:
	Character(sf::Color color);
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

	void setAmmosColor(sf::Color normalColor, sf::Color inactiveColor);
	sf::Color getInactiveAmmoColor() const;
	sf::Color getNormalAmmoColor() const;

	// On d�fini la direction du personnage gr�ce aux inputs
	void direction(int isleft, int isright, int up, int down, float deltaTime);

	bool hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime);
	bool characterCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const;

	sf::Vector2f shootDirection(sf::Vector2i mousePos);
	sf::Vector2f shootDepart();
	sf::Vector3f getPositionAndRadiusCharac();

	void ammoCount(int ammo);

	//Spawn
	void setPosition(int xSpawn, int ySpawn);
};
