#pragma once

#include <SFML/Graphics.hpp>

class PoPossibEngin;

class InputsManager
{
public:
	explicit InputsManager(PoPossibEngin& engine);

    void clearEvents();
	void updateEvents(sf::RenderWindow& renderWindow);

	[[nodiscard]] bool getEvent(sf::Event::EventType eventType);
	[[nodiscard]] bool getEvent(sf::Event::EventType eventType, sf::Event& outEvent);
	[[nodiscard]] sf::Vector2i getMousePosition();

	void setMousePosition();
	void update();

private:
    PoPossibEngin& _engine;

	const int _eventsSize = sf::Event::Count;
	sf::Event _events[sf::Event::Count]{};

    sf::Vector2<int> _mousePosition;
};