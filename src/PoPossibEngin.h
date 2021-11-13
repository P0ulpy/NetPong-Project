#pragma once

#include "SFML/Graphics.hpp"

class PoPossibEngin
{
	//Window and SFML stuff
	sf::VideoMode _videoMode;
	std::unique_ptr<sf::RenderWindow> _window;
	sf::Event _sfmlEvent;

	//Time - DeltaTime
	sf::Clock _dtClock;
	float _deltaTime;

	//Initializers
	void initWindow();

public:
	//Constructors - Destructors
	PoPossibEngin();
	virtual ~PoPossibEngin();

	//Accessors
	bool isRunning() const;

	//Functions
	void pollEvents();
	void handleKeyPressed(const sf::Keyboard::Key& keyPressed) const;

	void update();
	void updateDeltaTime();
	void render();
	void run();
};