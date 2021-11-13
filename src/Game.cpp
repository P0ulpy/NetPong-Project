#include "Game.h"


//--- Constructors - Destructors ---
Game::Game()
{
	initWindow();
	initVariables();
	initFonts();
}

Game::~Game()
{

}

//--- Initializers ---
void Game::initWindow()
{
	_videoMode = sf::VideoMode(1200, 900);
	_window = std::make_unique<sf::RenderWindow>(_videoMode, "Game 2", sf::Style::Close | sf::Style::Titlebar);
	_window->setFramerateLimit(60);
}

void Game::initVariables()
{
	_terrain = std::make_unique<Terrain>(*_window);
	_pongBall = std::make_unique<PongBall>(*_window, _terrain->getPlayableArea());
}

void Game::initFonts()
{
	if(!_font.loadFromFile("./Assets/arial.ttf"))
	{
		std::cout << "ERROR FONT NOT LOADED - Game.cpp" << std::endl;
	}
}

//--- Update - Render ---
void Game::update()
{
	pollEvents();

	_pongBall->update(*_window, _deltaTime);
}

void Game::updateDeltaTime()
{
	_deltaTime = _dtClock.restart().asSeconds();
}

void Game::render()
{
	_window->clear();

	_terrain->render(*_window);
	_pongBall->render(*_window);

	_window->display();
}

//--- Methods ---
void Game::pollEvents()
{
	while (_window->pollEvent(_sfmlEvent))
	{
		switch (_sfmlEvent.type)
		{
		case sf::Event::Closed:
			_window->close();
			break;
		case sf::Event::KeyPressed:
			handleKeyPressed(_sfmlEvent.key.code);
			break;
		default:
			break;
		}
	}
}

void Game::handleKeyPressed(const sf::Keyboard::Key& keyPressed) const
{
	switch (keyPressed)
	{
		case sf::Keyboard::Escape:
			_window->close();
			break;
		case sf::Keyboard::Space:
			_pongBall->resetSpeedBonusRatio();
			break;
		case sf::Keyboard::Numpad0:
			_pongBall->createPhantomBalls();
			break;
		case sf::Keyboard::Numpad1:
			_pongBall->activatedErasingLastPhantomBall();
			break;
		case sf::Keyboard::Numpad2:
			_pongBall->startPhantomBallEffect();
			break;
		case sf::Keyboard::Numpad3:
			_pongBall->boostBall(8.f);
			break;
		default:
			break;
	}
}

//--- Getters - Setters ---
bool Game::isRunning() const
{
	return _window->isOpen();
}
