#pragma once

#include <iostream>
#include <sstream>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "Terrain.h"
#include "PongBall.h"
/*
	Class that acts as the game engine.
	Wrapper class.
*/
class PongBall;
class Terrain;
class PongBall;

class Game
{
private:
	//Window and SFML stuff
	sf::VideoMode _videoMode;
	std::unique_ptr<sf::RenderWindow> _window;
	sf::Event _sfmlEvent;

	//Time - DeltaTime
	sf::Clock _dtClock;
	float _deltaTime;

	//Font and texts
	sf::Font _font;

	//Game objects
	std::unique_ptr<Terrain> _terrain;
	std::unique_ptr<PongBall> _pongBall;

	//Initializers
	void initWindow();
	void initVariables();
	void initFonts();

public:
	//Constructors - Destructors
	Game();
	~Game();

	//Accessors
	bool isRunning() const;

	//Modifiers

	//Functions
	void pollEvents();
	void handleKeyPressed(const sf::Keyboard::Key& keyPressed) const;

	void update();
	void updateDeltaTime();

	void render();
};
