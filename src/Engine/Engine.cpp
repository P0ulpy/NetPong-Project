#include <iostream>

#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "Engine.hpp"

// Scenes
#include "Scenes/MainGameScene.hpp"
#include "Scenes/SocketConnectionScene.hpp"

#include "../Logger/Logger.hpp"

PoPossibEngin* PoPossibEngin::_instance;

PoPossibEngin::PoPossibEngin(const EngineConfig& engineConfig)
    : _engineConfig(engineConfig)
	, _socketManager(new SocketManager(*this))
	, _renderThread(sf::Thread(&PoPossibEngin::renderThreadEntry, this))
	, _logicThread(sf::Thread(&PoPossibEngin::logicThreadEntry, this))
{
	if (_instance != nullptr)
        Logger::Err("Engine is a Singleton ! : Overriding old Engine intance");

    _instance = this;
}

void PoPossibEngin::start()
{
	_engineState = INITIALIZING;

	_renderThread.launch();
	_logicThread.launch();

	_renderThread.wait();
	_logicThread.wait();
}

void PoPossibEngin::stop()
{
	_engineState = STOP;

	delete _currScene;
	_currScene = nullptr;

	delete _socketManager;
	_socketManager = nullptr;

	_renderThread.terminate();
	_logicThread.terminate();

    ImGui::SFML::Shutdown();
    if(_renderWindow)
        _renderWindow->close();
}

PoPossibEngin::~PoPossibEngin()
{
	delete _renderWindow;
	delete _currScene;
	delete _socketManager;

}

#pragma region GET/SET

// Get

PoPossibEngin& PoPossibEngin::getInstance() { return *_instance; }

sf::RenderWindow& PoPossibEngin::getRenderWindow() const { return *_renderWindow; }
const EngineState& PoPossibEngin::getEngineState() const { return _engineState; }
const EngineConfig& PoPossibEngin::getEngineConfig() const { return _engineConfig; }

sf::Thread& PoPossibEngin::getRenderThread() { return _logicThread; }
sf::Thread& PoPossibEngin::getLogicThread() { return _renderThread; }

float PoPossibEngin::getDeltaTime() const { return _deltaTime; }

InputsManager& PoPossibEngin::getInputsManager() { return _inputsManager; }
SocketManager* PoPossibEngin::getSocketManager() { return _socketManager; }

#pragma endregion GET/SET

void PoPossibEngin::pollEvents()
{
	_inputsManager.updateEvents(*_renderWindow);

	if(_inputsManager.getEvent(sf::Event::Closed))
	{
		stop();
	}
}

#pragma region RenderThread

void PoPossibEngin::renderThreadEntry()
{
    Logger::SetThreadLabel("Render-Thread");
	Logger::Log("Render Thread Entry");

	renderThread_InitWindow();

	Logger::Log("Engine Initilized");
	_engineState = INITIALIZED;

	// TEMPORAIRE
	loadScene(SocketConnection);

	renderThreadUpdate();
}

void PoPossibEngin::renderThread_InitWindow()
{
	_renderWindow = new sf::RenderWindow(
		_engineConfig.windowConfig.videoMode,
		_engineConfig.windowConfig.title,
		_engineConfig.windowConfig.style
	);

	if (_engineConfig.windowConfig.framerateLimit != 0)
	{
		_renderWindow->setFramerateLimit(_engineConfig.windowConfig.framerateLimit);
	}
	else
	{
		_renderWindow->setVerticalSyncEnabled(true);
	}

	ImGui::SFML::Init(*_renderWindow);
}

void PoPossibEngin::renderThreadUpdate()
{
	if(!_renderWindow)
	{
        Logger::Err("_renderWindow not initialized, stopping");
        return;
	}

    while (_renderWindow->isOpen())
	{
		if (_engineState == RUNNING)
		{
			sf::Time _deltaTime_Time = _deltaClock.restart();
			_deltaTime = _deltaTime_Time.asSeconds();
			ImGui::SFML::Update(*_renderWindow, _deltaTime_Time);

			// DEBUG INFOS
			renderThreadDebugInfo();

			_renderWindow->clear();

			// TEMP
			_currScene->update(getDeltaTime());
			// TEMP

			_currScene->render(_renderWindow);
			ImGui::SFML::Render(*_renderWindow);

			_renderWindow->display();

			pollEvents();
		}
	}
}

void PoPossibEngin::renderThreadDebugInfo()
{
	ImGui::Begin("PoPosibEngin Info");

	_currFrameCount++;
	_currFrameTimeCount += _deltaTime;

	if (_currFrameTimeCount >= 1)
	{
		_currFrameRate = _currFrameCount;
		_currFrameCount = 0;
		_currFrameTimeCount = 0;
	}
	
	ImGui::Text("FrameRate : %d", _currFrameRate);
	ImGui::Text("DeltaTime : %.5f", (double)getDeltaTime());
	ImGui::Text("Engine state : %d", _engineState);

	ImGui::End();
}

#pragma endregion RenderThread


#pragma region LogicThread

void PoPossibEngin::logicThreadEntry()
{
    Logger::SetThreadLabel("Logic-Thread");
    Logger::Log("Logic Thread Entry");
}

void PoPossibEngin::logicThreadUpdate()
{
	//throw std::exception("not implemented");
}

#pragma endregion LogicThread

void PoPossibEngin::loadScene(SceneType sceneType)
{
	_engineState = PAUSE;

	Scene* newScene;

	switch (sceneType)
	{
	case SocketConnection: 
		newScene = new SocketConnectionScene(*this);
		break;
	case MainGame:
		newScene = new MainGameScene(*this);
		break;
	}

	delete _currScene;
	_currScene = newScene;

	_engineState = RUNNING;

	_currScene->start();
}
