#pragma once

#include "SFML/Graphics.hpp"

class PongBall;

class PhantomBall
{
public:
	//Constructors - Destructors
	PhantomBall(const PongBall& ballParent);
	virtual ~PhantomBall();

	//Functions
	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void show();
	void hide();

	void setFillColor(const sf::Color& newColor);
	bool isDisplayed() const;

private:
	//Attributes
	const PongBall& _pongBallParent;
	sf::Color _currentFillColor;
	sf::Color _currentOutlineColor;
	sf::CircleShape _phantomBallShape;

	sf::Uint8 _currentAlpha;

	float _timeAlphaDecreaseCooldown;

	bool _isDisplayed { false };

	//Initializers
	void initVariables();
	void initShapeAndColor();
	void resetTimer();

	void setAndUpdateShapeAlpha(const sf::Uint8& alpha);
};