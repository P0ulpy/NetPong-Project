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
		sf::VideoMode videoMode = sf::VideoMode(1200, 900);
		
		std::string title = "Render Window";
		unsigned int style = sf::Style::Default;
		unsigned int framerateLimit = 60; // 0 mean automatic (verticalSyncEnabled = true)

		WindowConfig(sf::VideoMode pVideoMode, std::string pTitle = "Render Window", unsigned int pStyle = sf::Style::Default, unsigned int pFramerateLimit = 60)
			: videoMode(pVideoMode), title(pTitle), style(pStyle), framerateLimit(pFramerateLimit) {}
	};

	WindowConfig windowConfig;
	
	EngineConfig(WindowConfig pWindowConfig)
		: windowConfig(pWindowConfig) {}
};

enum EngineState { STOP, INITIALIZING, INITIALIZED, RUNNING, PAUSE };

// TEMP
enum SceneType { SocketConnection, MainMenu, MainGame };

class PoPossibEngin
{
public:
	PoPossibEngin() = default;
	PoPossibEngin(const EngineConfig& engineConfig);

	PoPossibEngin(const sf::Thread &renderThread);

	~PoPossibEngin();

	void start();
	void stop();

	// get / set

	//[[nodiscard]] static PoPossibEngin& getInstance();
	sf::Vector2i getMousePosition();

	
	[[nodiscard]] sf::RenderWindow& getRenderWindow() const;
	[[nodiscard]] const EngineState& getEngineState() const;
	[[nodiscard]] const EngineConfig& getEngineConfig() const;

    [[maybe_unused]] [[nodiscard]] sf::Thread& getRenderThread();
	[[nodiscard]] sf::Thread& getLogicThread();

	[[nodiscard]] float getDeltaTime() const;

	[[nodiscard]] InputsManager& getInputsManager();
	[[nodiscard]] SocketManager& getSocketManager();
	void setMousePosition();

private:

	//static PoPossibEngin* _instance;

	sf::Vector2i mousePosition;

	

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
