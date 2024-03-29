#include "SFML/Graphics.hpp"
#include "../Entity.hpp"
#include "../../Engine/Interfaces/IControllable.hpp"
#include "../../Engine/Interfaces/IRenderable.hpp"
#include <mutex>

class PongBall;

class Character : public Entity, public Engine::IControllable, public Engine::IRenderable
{
private:

    std::mutex _mutex;
    bool _canCharacterMove { true };

	sf::CircleShape charac;
	sf::RectangleShape canon;
	sf::CircleShape shootZone;

    std::vector<PongBall*> _pongBalls;

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

	//Joueur mort
	bool _isAlive = true;

public:
    Character() = default;
    Character(sf::Color color);
	~Character() override = default;

    void update(const float& deltaTime) override;
	void render(sf::RenderTarget& renderTarget) const override;
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime) override;

    bool isInCooldown() const;
    bool isReloading() const;

	void activateCooldown(bool activate);
	void activateReloading(bool activateReload);

    sf::Vector2f shootDepart() const;
    sf::Vector2f shootDirection(sf::Vector2i mousePos) const;

	bool hitWallIfCollision(float x1, float y1, float x2, float y2);
	bool characterCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint) const;

    // GET
    [[nodiscard]] std::vector<PongBall*> getPongBalls();
    [[nodiscard]] const sf::CircleShape& getShape() const;
    [[nodiscard]] const sf::RectangleShape& getCanon() const;
    [[nodiscard]] sf::Vector2f getVelocity() final;
    [[nodiscard]] sf::Vector2i getPosition() const final;
    [[nodiscard]] float getRotation() const final;
    [[nodiscard]] sf::Vector3f getPositionAndRadiusCharac();
    [[nodiscard]] sf::Color getInactiveAmmoColor() const;
    [[nodiscard]] sf::Color getNormalAmmoColor() const;
    [[nodiscard]] float getRadius() const;

    // SET
    void setPosition(const sf::Vector2i& position) final;
    void setRotation(float rot) final;
    void setVelocity(const sf::Vector2f& newVelocity) final;
    void setAmmosColor(sf::Color normalColor, sf::Color inactiveColor);

    void setPlayerAlive(bool isAlive);
    void ammoCount(int ammo);
    void toggleCharacterMove(bool canCharacterMove);
	bool canCharacterMove() const;
	bool canCharacterShoot() const;
	void resetAmmos();

    void addPongBall(PongBall* pongBall = nullptr);
    PongBall* getOneInactivePongball() const;
};
