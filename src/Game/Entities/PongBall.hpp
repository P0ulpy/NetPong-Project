#pragma once

#include "SFML/Graphics.hpp"

#include "../Entity.hpp"

class PolygonTerrain;
class PhantomBallEffect;
class PolygonCollisionResult;
class MainGameScene;

class PongBall
{
public:
	//Constructors - Destructors
	PongBall(const sf::RenderWindow& window, const sf::Rect<float>& terrain, PolygonTerrain& polyTerrain, MainGameScene& mainGameScene);
	virtual ~PongBall();

	//Functions
	void update(const float& deltaTime);
	void updateCollision(const float& deltaTime);
	void updateMovement(const float& deltaTime);
	void updateBoost(const float& deltaTime);
	void updatePhantomEffect(const float& deltaTime);

	void render(sf::RenderTarget& target) const;
	void renderPhantomEffect(sf::RenderTarget& target) const;

	bool hitWallIfCollision(float x1, float y1, float x2, float y2, float& remainingTime, const float& deltaTime);

	void shoot(sf::Vector2f position, sf::Vector2f normVelocity, sf::Color color = sf::Color::White);

	//Getters - Setters
	sf::CircleShape getShape() const;

	void setSpeed(float speed);

	void setActive(bool isActive);
	bool isActive() const;

	void setCanKill(bool canKill);
	bool canKill() const;

	//Boost and collision
	void startBoostBall(float speedBoostBonus);
	void setSpeedMultiplierBonus(float pSpeedMultiplierBonus);
	void resetSpeedMultiplierBonus();

	bool linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint, float& remainingTime) const;
	void resetBallDestAndOldPos(const float& deltaTime);

	void addNumBounceAndUpdateVisibility();
	void stopPhantomBallEffect();

private:
	//MainGameScene
	MainGameScene* _mainGameScene;

	//Ball rendering
	bool _isActive;

	sf::CircleShape _ballShape;
	sf::Color _ballColor;
	sf::Color _ballInactiveColor;
	int _ballSize;

	sf::Vector2f _initialPosition;
	sf::CircleShape* _ballDestination;
	sf::Vector2f _oldPosition;

	//Ball physics
	sf::Vector2f _velocity;
	float _currentSpeed;
	float _initialSpeed;
	float _maxSpeed;

	float _speedMultiplierBonus;
	int _currentNumBounces;
	bool _canKill;

	//Terrain
	sf::Rect<float> _terrainArea;
	PolygonTerrain* _polygonTerrain;

	//Boost
	float _currentTimeBoost;
	bool _isBoosted;

	//Phantom ball effect
	std::unique_ptr<PhantomBallEffect> _phantomBallEffect;

	//Functions
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime);

	//Initializers
	void initVariables();
	void initShapes(const sf::RenderWindow& window);
	void initBoost();
	void initPhantomEffect();
};