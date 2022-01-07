#pragma once

#include "SFML/Graphics.hpp"

#include "../Entity.hpp"

constexpr int BALL_SIZE = 20;

class PolygonTerrain;
class PhantomBall;
class PolygonCollisionResult;

class PongBall : public Entity
{
public:
	//Constructors - Destructors
	PongBall(const sf::RenderWindow& window, const sf::Rect<float>& terrain, PolygonTerrain& polyTerrain);

	virtual ~PongBall();

	//Functions
	void update(const float& deltaTime) override;
	void updateCollision(const float& deltaTime);
	void updateMovement(const float& deltaTime);
	void updateBoost(const float& deltaTime);
	void updatePhantomEffect(const float& deltaTime);

	void render(sf::RenderTarget& target) const override;
	void renderPhantomEffect(sf::RenderTarget& target) const;

	void updateAndRenderPhantomEffect(sf::RenderTarget& target, const float& deltaTime);

	bool hitWallIfCollision(float x1, float y1, float x2, float y2);
	void resetBallDestAndOldPos(const float& deltaTime);

	//Getters - Setters
	sf::CircleShape getShape() const;

	void setSpeed(float speed);
	void setSpeedMultiplierBonus(float pSpeedMultiplierBonus);
	void addSpeedMultiplierBonus(float pSpeedMultiplierBonus);
	void resetSpeedMultiplierBonus();

	void setActive(bool isActive);
	bool isActive() const;

	void startPhantomBallEffect();
	void stopPhantomBallEffect();

	void startBoostBall(float speedBoostBonus);

	bool linePongBallCollision(float x1, float y1, float x2, float y2, sf::Vector2f& outImpactPoint) const;

private:
	//Ball rendering
	bool _isActive;

	sf::CircleShape _ballShape;
	sf::Color _ballColor;
	int _ballSize;

	sf::Vector2f _initialPosition;
	sf::CircleShape* _ballDestination;
	sf::Vector2f _oldPosition;

	//Ball physics
	float _speedMultiplierBonus;
	int _maxNumBounces;
	int _currentNumBounces;

	//Terrain
	sf::Rect<float> _terrainArea;
	PolygonTerrain* _polygonTerrain;

	//Boost
	float _currentTimeBoost;
	float _boostDuration;
	bool _isBoosted;

	//Phantom ball effect
	std::vector<std::unique_ptr<PhantomBall>> _phantomBalls;
	int _phantomBallsMax;

	float _currentTimePhantomBallCooldown;
	float _durationBetweenPhantomBalls;
	bool _hasPhantomEffect;

	//Functions
	void moveEntity(const sf::Vector2f& velocity, const float& deltaTime) override;

	//Initializers
	void initVariables();
	void initShapes(const sf::RenderWindow& window);
	void initBoost();
	void initPhantomEffect();

	//Phantom Ball Effect
	void displayPhantomBall();
	void createPhantomBalls();

	void addNumBounceAndUpdateVisibility();
};