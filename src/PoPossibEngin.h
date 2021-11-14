#pragma once

#include "MainGameScene.h"

class Scene;
class MainGainScene;

class PoPossibEngin
{
private:
	//Window and SFML stuff
	sf::VideoMode _videoMode;
	std::unique_ptr<sf::RenderWindow> _window;
	sf::Event _sfmlEvent;

	//Scenes
	std::stack<std::unique_ptr<Scene>> _scenes;//On utilise une stack, c'est bien ?

	//Time - DeltaTime
	sf::Clock _dtClock;
	float _deltaTime;

	//Initializers
	void initWindow();
	void initScenes();

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

	void quitApplication() const;
};