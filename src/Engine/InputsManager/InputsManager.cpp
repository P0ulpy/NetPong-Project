#include "imgui-SFML.h"
#include "../Engine.hpp"
#include "InputsManager.hpp"


InputsManager::InputsManager(PoPossibEngin& engine)
    : _engine(engine)
{
    clearEvents();
}

void InputsManager::clearEvents()
{
	for (int i = 0; i < _eventsSize; i++)
	{
		_events[i].type = sf::Event::Count;
	}
}

void InputsManager::updateEvents(sf::RenderWindow& renderWindow)
{
	clearEvents();

	sf::Event event {};
	while(renderWindow.pollEvent(event))
	{
		// ImGui Events Handling
		ImGui::SFML::ProcessEvent(event);

		_events[event.type] = event;
	}
}

bool InputsManager::getEvent(sf::Event::EventType eventType)
{
	return (_events[eventType].type != sf::Event::Count);
}

bool InputsManager::getEvent(sf::Event::EventType eventType, sf::Event& outEvent)
{
	if (_events[eventType].type != sf::Event::Count)
	{
		outEvent = _events[eventType];
		return true;
	}

	return false;
}

void InputsManager::update()
{
	setMousePosition();
}

//Mouse
void InputsManager::setMousePosition() { _mousePosition = sf::Mouse::getPosition(_engine.getRenderWindow()); }
sf::Vector2i InputsManager::getMousePosition() { return _mousePosition; }
