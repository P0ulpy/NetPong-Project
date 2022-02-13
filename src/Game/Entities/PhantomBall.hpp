#pragma once

#include "SFML/Graphics.hpp"

class PhantomBallEffect;
class PongBall;

class PhantomBall
{
public:
	//Constructors - Destructors
	PhantomBall(PhantomBallEffect* phantomBallEffectParent, bool isActive = true);
	virtual ~PhantomBall();

	//Functions
	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void setIsActive(bool isActive);

	void setFillColor(const sf::Color& newColor);
	bool isActive() const;

private:
	//Attributes
	PhantomBallEffect* _phantomBallEffectParent;

	sf::Color _currentFillColor;
	sf::Color _currentOutlineColor;
	sf::CircleShape _phantomBallShape;

	sf::Uint8 _currentAlpha;

	float _timeAlphaDecreaseCooldown = 0;

	bool _isActive ;

	//Initializers
	void initVariables();
	void initShapeAndColor();
	void resetTimer();

	void show();
	void hide();

	void setAndUpdateShapeAlpha(const sf::Uint8& alpha);
};