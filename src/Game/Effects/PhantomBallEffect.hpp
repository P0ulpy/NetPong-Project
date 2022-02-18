#pragma once
#include <memory>
#include <vector>
#include <stack>

#include "SFML/Graphics/RenderTarget.hpp"

class PongBall;
class PhantomBall;

class PhantomBallEffect
{
private:
	const PongBall& _ballParent;
	std::vector<PhantomBall*> _phantomBalls;
	float _currentTimePhantomBallCooldown;
	bool _hasPhantomEffect;

public:
	PhantomBallEffect(const PongBall& ballParent);
	~PhantomBallEffect();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void begin();
	void stop();

	void displayPhantomBall() const;
	void createPhantomBalls();

	void setPhantomBallFillColour(const sf::Color& newColor) const;
	[[nodiscard]] const PongBall& getPongBallParent() const;
};