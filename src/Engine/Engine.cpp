
#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "Engine.hpp"

// Scenes
#include "Scenes/MainGameScene.hpp"

PoPossibEngin::PoPossibEngin(const EngineConfig& engineConfig)
// Threads Init
	: _renderThread(sf::Thread(&PoPossibEngin::renderThreadEntry, this)),
		_logicThread(sf::Thread(&PoPossibEngin::logicThreadEntry, this))
{
	_renderWindow = new sf::RenderWindow(
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

	if(engineConfig.windowConfig.enableImgui)
	{
		//ImGui::SFML::Init(*_renderWindow);
	}

	std::cout << "Engine Initilized" << std::endl;
	_engineState = INITIALIZED;

	// TEMPORAIRE
	loadScene(MainGame);

	// Threads Init

	//_renderThread.launch();
	//_logicThread.launch();
}

PoPossibEngin::~PoPossibEngin()
{
	delete _renderWindow;
	delete _currScene;
}

sf::RenderWindow& PoPossibEngin::getRenderWindow() const { return *_renderWindow; }
EngineState PoPossibEngin::getEngineState() const { return _engineState; }

sf::Thread& PoPossibEngin::getRenderThread() { return _logicThread; }
sf::Thread& PoPossibEngin::getLogicThread() { return _renderThread; }

void PoPossibEngin::refreshDeltaTime()
{
	_deltaTime = _deltaClock.restart().asSeconds();
}

void PoPossibEngin::renderThreadEntry()
{
	std::cout << "Render Thread Entry" << std::endl;
	sf::Event event;
	while(_renderWindow->isOpen())
	{
		if(_engineState == RUNNING)
		{
			refreshDeltaTime();
		
			std::cout << "Delta Time : " << _deltaTime << std::endl;
			while (_renderWindow->pollEvent(event))
			{


			}
			_renderWindow->clear();

			_currScene->update(_deltaTime);

			// TEMPORAIRE
			_currScene->render(_renderWindow);
			// TEMPORAIRE

			_renderWindow->display();
		}
	}
}

void PoPossibEngin::logicThreadEntry()
{
	std::cout << "Render Thread Entry" << std::endl;

	/*while (_renderWindow->isOpen())
	{
		if (_engineState == RUNNING)
		{
			// A REMPLIR
		}
	}*/
}

void PoPossibEngin::loadScene(SceneType sceneType)
{
	{
		_engineState = PAUSE;

		Scene* newScene;

		switch (sceneType)
		{
		case MainGame: 
			newScene = new MainGameScene(*this);
			break;
		}

		delete _currScene;
		_currScene = newScene;

		_engineState = RUNNING;
	}
}

#endif //ENGINE_CPP