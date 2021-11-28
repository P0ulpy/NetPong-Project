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
	void updateCollision();
	void updateMovement(const float& deltaTime);
	void updateBoost(const float& deltaTime);
	void updatePhantomEffect(const float& deltaTime);

	void render(sf::RenderTarget& target) const override;
	void renderPhantomEffect(sf::RenderTarget& target) const;

	void updateAndRenderPhantomEffect(sf::RenderTarget& target, const float& deltaTime);

	//Getters - Setters
	sf::CircleShape getShape() const;

	void setSpeed(float speed);
	void setSpeedMultiplierBonus(float pSpeedMultiplierBonus);
	void addSpeedMultiplierBonus(float pSpeedMultiplierBonus);
	void resetSpeedMultiplierBonus();

	void startPhantomBallEffect();
	void stopPhantomBallEffect();

	void startBoostBall(float speedBoostBonus);

	static float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	static sf::Vector2f normalize(const sf::Vector2f& originalVector);

private:
	//Ball rendering
	sf::CircleShape _ballShape;
	sf::Color _ballColor;
	int _ballSize;

	//Ball physics
	float _speedMultiplierBonus;

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

	//Utils
	float deceleration(float initial, float target, float time) const;
	bool lineCircleCollision(float x1, float y1, float x2, float y2, float cx, float cy, float r, sf::Vector2f& outImpactPoint) const;
	bool pointCircleCollision(float px, float py, float cx, float cy, float r) const;
	bool linePointCollision(float x1, float y1, float x2, float y2, float px, float py) const;
	float getDistance(float x1, float y1, float x2, float y2) const;
};