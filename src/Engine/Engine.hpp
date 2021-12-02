#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "InputsManager/InputsManager.hpp"
#include "SocketsManagement/SocketsManager.hpp"

class Scene;

struct EngineConfig
{
	struct WindowConfig
	{
		sf::VideoMode videoMode = sf::VideoMode(800, 800);
		std::string title = "Render Window";
		unsigned int style = sf::Style::Default;
		unsigned int framerateLimit = 0; // 0 mean automatic (verticalSyncEnabled = true)
	};

	WindowConfig windowConfig;
};

enum EngineState { STOP, INITIALIZING, INITIALIZED, RUNNING, PAUSE };

// TEMP
enum SceneType { SocketConnection, MainGame };

class PoPossibEngin
{
public:
	PoPossibEngin(const EngineConfig& engineConfig);
	~PoPossibEngin();

	void start();
	void stop();

	// get / set

	[[nodiscard]] sf::RenderWindow& getRenderWindow() const;
	[[nodiscard]] const EngineState& getEngineState() const;
	[[nodiscard]] const EngineConfig& getEngineConfig() const;

	[[nodiscard]] sf::Thread& getRenderThread();
	[[nodiscard]] sf::Thread& getLogicThread();

	[[nodiscard]] float getDeltaTime() const;

	[[nodiscard]] SocketManager& getSocketManager();

private:

	EngineState _engineState = STOP;
	const EngineConfig& _engineConfig;

	sf::RenderWindow* _renderWindow = nullptr;

	Scene* _currScene = nullptr;

	sf::Clock _deltaClock;

	// TEMP : utilisation de _deltaTime pour le fix du soucis de la valeur de la clock incorecte lors de l'appel de .restart()
	float _deltaTime = 0;

	void pollEvents();

	// InputManager

	InputsManager _inputManager;

	// Threads

	sf::Thread _renderThread;
	void renderThreadEntry();
	void renderThread_InitWindow();
	void renderThreadUpdate();

	sf::Thread _logicThread;
	void logicThreadEntry();
	void logicThreadUpdate();

	void loadScene(SceneType sceneType);

	// Net

	SocketManager _socketManager;

	// DEBUG

	void renderThreadDebugInfo();

	float _currFrameTimeCount = 0;
	int _currFrameCount = 0;
	int _currFrameRate = 0;
};