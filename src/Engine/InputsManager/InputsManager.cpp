#include <SFML/System.hpp>

#include "imgui-SFML.h"

#include "InputsManager.hpp"

InputsManager::InputsManager()
{
	for (int i = 0; i < _eventsSize; i++)
	{
		_events[i] = nullptr;
	}
}

InputsManager::~InputsManager()
{
	for (int i = 0; i < _eventsSize; i++)
	{
		if (_events[i] != nullptr)
		{
			delete _events[i];
		}
	}
}

void InputsManager::updateEvents(sf::RenderWindow& renderWindow)
{

	for (int i = 0; i < _eventsSize; i++)
	{
		if (_events[i] != nullptr)
		{
			delete _events[i];
		}

		_events[i] = nullptr;
	}

	sf::Event event;
	while(renderWindow.pollEvent(event))
	{
		// ImGui Events Handling
		ImGui::SFML::ProcessEvent(event);

		_events[event.type] = new sf::Event(event);
	}
}

bool InputsManager::getEvent(sf::Event::EventType eventType)
{
	return (_events[eventType] != nullptr);
}

bool InputsManager::getEvent(sf::Event::EventType eventType, sf::Event& outEvent)
{
	if (_events[eventType] != nullptr)
	{
		outEvent = sf::Event(*_events[eventType]);
		return true;
	}

	return false;
}