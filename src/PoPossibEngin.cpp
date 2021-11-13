#include "PoPossibEngin.h"

#include "Game.h"


//--- Constructors - Destructors ---
PoPossibEngin::PoPossibEngin()
{
	initWindow();
}

PoPossibEngin::~PoPossibEngin()
{

}

//--- Initializers ---
void PoPossibEngin::initWindow()
{
	_videoMode = sf::VideoMode(1200, 900);

	_window = std::make_unique<sf::RenderWindow>(_videoMode, "Net-Pong Project", sf::Style::Close | sf::Style::Titlebar);
	_window->setFramerateLimit(60);
}

//--- Update - Render ---
void PoPossibEngin::update()
{
	pollEvents();
}

void PoPossibEngin::updateDeltaTime()
{
	_deltaTime = _dtClock.restart().asSeconds();
}

void PoPossibEngin::render()
{
	_window->clear();

	_window->display();
}

void PoPossibEngin::run()
{
	while (isRunning())
	{
		updateDeltaTime();
		update();
		render();
	}
}

//--- Methods ---
void PoPossibEngin::pollEvents()
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

void PoPossibEngin::handleKeyPressed(const sf::Keyboard::Key& keyPressed) const
{
	switch (keyPressed)
	{
		case sf::Keyboard::Escape:
			_window->close();
			break;
		default:
			break;
	}
}

//--- Getters - Setters ---
bool PoPossibEngin::isRunning() const
{
	return _window->isOpen();
}
