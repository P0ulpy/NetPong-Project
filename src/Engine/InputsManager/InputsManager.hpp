#pragma once

#include <SFML/Graphics.hpp>

class InputsManager
{
public:
	InputsManager();
	~InputsManager();

	void updateEvents(sf::RenderWindow& renderWindow);

	[[nodiscard]] bool getEvent(sf::Event::EventType eventType);
	[[nodiscard]] bool getEvent(sf::Event::EventType eventType, sf::Event& outEvent);
private:
	const int _eventsSize = sf::Event::Count;
	sf::Event* _events[sf::Event::Count];
};