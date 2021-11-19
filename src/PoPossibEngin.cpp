#include "PoPossibEngin.h"
#include "MainGameScene.h"

//--- Constructors - Destructors ---
PoPossibEngin::PoPossibEngin()
{
	initWindow();
	initScenes();
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

void PoPossibEngin::initScenes()
{
	_scenes.push(std::make_unique<MainGameScene>(*this));
}

//--- Update - Render ---
void PoPossibEngin::update()
{
	pollEvents();

	if (!_scenes.empty())
	{
		_scenes.top()->update(_deltaTime);
	}
}

void PoPossibEngin::updateDeltaTime()
{
	_deltaTime = _dtClock.restart().asSeconds();
}

void PoPossibEngin::render()
{
	_window->clear();

	if(!_scenes.empty())
	{
		_scenes.top()->render(_window.get());
	}

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

void PoPossibEngin::quitApplication() const
{
	_window->close();
}

//--- Methods ---
void PoPossibEngin::pollEvents()
{
	while (_window->pollEvent(_sfmlEvent))
	{
		switch (_sfmlEvent.type)
		{
		case sf::Event::Closed:
			quitApplication();
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
			quitApplication();
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

sf::RenderWindow& PoPossibEngin::getRenderWindow() const
{
	return *_window;
}
