#pragma once
#include <memory>
#include <vector>

#include "SFML/Graphics/RenderTarget.hpp"

class PongBall;
class PhantomBall;

class PhantomBallEffect
{
private:
	const PongBall& _ballParent;
	std::vector<std::unique_ptr<PhantomBall>> _phantomBalls;
	float _currentTimePhantomBallCooldown;
	bool _hasPhantomEffect;

public:
	PhantomBallEffect(const PongBall& ballParent);
	~PhantomBallEffect();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void begin();
	void stop();

	void displayPhantomBall();
	void createPhantomBalls();

	void setPhantomBallFillColour(const sf::Color& newColor) const;
};