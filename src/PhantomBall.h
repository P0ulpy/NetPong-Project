#pragma once

#include "SFML/Graphics.hpp"
#include "Game.h"

class PongBall;

class PhantomBall
{
public:
	//Constructors - Destructors
	PhantomBall(PongBall& ballParent);
	virtual ~PhantomBall();

	//Functions
	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void displayPhantomBall();
	void hidePhantomBall();
	bool isDisplayed() const;

private:
	//Attributes
	PongBall& _pongBallParent;
	sf::Color _currentFillColor;
	sf::Color _currentOutlineColor;
	sf::CircleShape _phantomBallShape;

	sf::Uint8 _initialAlpha;
	sf::Uint8 _currentAlpha;

	float _alphaDecreaseCooldown;
	int _alphaDecreaseSpeed;

	float _timeAlphaDecreaseCooldown;

	bool _isDisplayed{ false };

	//Initializers
	void initVariables();
	void initShapeAndColor();
	void resetTimer();
};
