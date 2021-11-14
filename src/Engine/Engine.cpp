#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "Engine.hpp"

using namespace std;
using namespace sf;

Engine::Engine(const EngineConfig& engineConfig)
// Threads Init
: _renderThread(Thread(&Engine::renderThreadEntry, this)), _logicThread(Thread(&Engine::logicThreadEntry, this))
{
	_renderWindow = new RenderWindow(
		engineConfig.windowConfig.videoMode, 
		engineConfig.windowConfig.title, 
		engineConfig.windowConfig.style
	);

	if(engineConfig.windowConfig.framerateLimit)
	{
		_renderWindow->setFramerateLimit(engineConfig.windowConfig.framerateLimit);
	}
	else
	{
		_renderWindow->setVerticalSyncEnabled(true);
	}

	if(engineConfig.windowConfig.enableIMGUI)
	{
		ImGui::SFML::Init(*_renderWindow);
	}
}

Engine::~Engine()
{
	delete _renderWindow;
}

sf::RenderWindow& Engine::getRenderWindow() const
{
	return *_renderWindow;
}

void Engine::renderThreadEntry()
{

}

void Engine::logicThreadEntry()
{

}

#endif //ENGINE_CPP