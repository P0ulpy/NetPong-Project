#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <utility>
#include <mutex>

#include "InputsManager/InputsManager.hpp"
#include "SocketsManagement/SocketsManager.hpp"

class Scene;

struct EngineConfig
{
	struct WindowConfig
	{
		sf::VideoMode videoMode = sf::VideoMode(1200, 900);
		
		std::string title = "Render Window";
		unsigned int style = sf::Style::Fullscreen;
		unsigned int framerateLimit = 60; // 0 mean automatic (verticalSyncEnabled = true)

		explicit WindowConfig(sf::VideoMode pVideoMode, std::string pTitle = "Render Window", unsigned int pStyle = sf::Style::Fullscreen, unsigned int pFramerateLimit = 60)
			: videoMode(pVideoMode), title(std::move(pTitle)), style(pStyle), framerateLimit(pFramerateLimit) {}
	};

	WindowConfig windowConfig;

    explicit EngineConfig(WindowConfig pWindowConfig);
};

enum EngineState { STOP, INITIALIZING, INITIALIZED, RUNNING, PAUSE };

// TEMP
enum SceneType { SocketConnection, MainMenu, MainGame, None };

class PoPossibEngin
{
public:
	explicit PoPossibEngin(const EngineConfig& engineConfig);
    ~PoPossibEngin();

	void start();
	void stop();

    void loadScene(SceneType sceneType);

    [[nodiscard]] static PoPossibEngin& getInstance();
	
	[[nodiscard]] sf::RenderWindow& getRenderWindow() const;
	[[nodiscard]] const EngineState& getEngineState() const;
	[[nodiscard]] const EngineConfig& getEngineConfig() const;

    [[maybe_unused]] [[nodiscard]] sf::Thread& getRenderThread();
	[[nodiscard]] sf::Thread& getLogicThread();

	[[nodiscard]] float getDeltaTime() const;

	[[nodiscard]] InputsManager& getInputsManager();
	[[nodiscard]] SocketManager* getSocketManager();

private:

	static PoPossibEngin* _instance;
    const EngineConfig& _engineConfig;

	EngineState _engineState = STOP;
	sf::RenderWindow* _renderWindow = nullptr;

	Scene* _currScene = nullptr;
	sf::Clock _deltaClock;

	float _deltaTime = 0;

    // Scenes Management
    void loadScene_internal(SceneType sceneType);
    SceneType _sceneToLoad = SceneType::MainMenu;

	// InputManager
	InputsManager _inputsManager { InputsManager(*this) };
    void pollEvents();

    // Threads

    std::mutex _mutex;

	sf::Thread _renderThread;
	void renderThreadEntry();
	void renderThread_InitWindow();
	void renderThreadUpdate();

	sf::Thread _logicThread;
	void logicThreadEntry();
	void logicThreadUpdate();

	// Net
	SocketManager* _socketManager;

	// DEBUG
	void renderThreadDebugInfo();
	float _currFrameTimeCount = 0;
	int _currFrameCount = 0;
	int _currFrameRate = 0;
};
