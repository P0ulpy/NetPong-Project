#pragma once

#include "SFML/Graphics.hpp"

#include "PongBall.hpp"

class PhantomBall;

class PongBall
{
public:
	//Constructors - Destructors
	PongBall(const sf::RenderWindow& window, const sf::Rect<float>& terrain);
	virtual ~PongBall();

	//Functions
	void update(sf::RenderTarget& target, float deltaTime);
	void updateMovement(const float& deltaTime);
	void updatePhantomEffect(const float& deltaTime);

	void render(sf::RenderTarget& target) const;
	void renderPhantomEffect(sf::RenderTarget& target) const;

	//Getters - Setters
	sf::CircleShape getShape() const;

	void setTerrainArea(const sf::Rect<float>& terrain);

	float getCurrentBoostTimeStamp() const;
	void setSpeed(float speed);

	void setSpeedBonusRatio(float pSpeedBonusRatio);
	void addSpeedBonusRatio(float pSpeedBonusRatio);
	void resetSpeedBonusRatio();

	void createPhantomBalls();
	void displayPhantomBall();
	void activatedErasingLastPhantomBall();

	void startPhantomBallEffect();

	void boostBall(float speedBoost);
	float deceleration(float initial, float target, float time) const;
	sf::Vector2f normalize(const sf::Vector2f& originalVector) const;
private:

	//Ball rendering
	sf::CircleShape _ballShape;
	sf::Color _ballColor;
	int _ballSize;

	//Ball physics
	sf::Vector2f _velocity;
	float _currentSpeed;
	float _initialSpeed;
	float _maxSpeed;
	float _speedBonusRatio;
	bool _needErasePhantomBall{ false };

	//Boost
	bool _isBoosted{ false };
	float _currentTimeBoost;
	float _boostDuration;

	//Phantom ball effect
	std::vector<std::unique_ptr<PhantomBall>> _phantomBalls;
	int _phantomBallsMax;

	float _currentTimePhantomBallEffect;
	float _currentTimePhantomBallCooldown;
	float _timeBetweenPhantomBalls;
	float _phantomBallEffectDuration;
	bool _hasPhantomEffect{ false };

	//Terrain
	sf::Rect<float> _terrainArea;

	//Functions
	void moveBall(const float& deltaTime);

	//Initializers
	void initVariables();
	void initShape(const sf::RenderWindow& window);
	void initBoost();
	void initPhantomEffect();

	void eraseLastPhantomBall();
};