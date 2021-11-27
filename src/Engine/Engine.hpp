#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Scenes/Scene.hpp"

class Scene;

struct EngineConfig
{
	struct WindowConfig
	{
		sf::VideoMode videoMode = sf::VideoMode(1200, 900);
		std::string title = "Render Window";
		unsigned int style = sf::Style::Default;
		bool enableImgui = true;
		unsigned int framerateLimit = 0; // 0 mean automatic (verticalSyncEnabled = true)
	};

	WindowConfig windowConfig;
};

enum EngineState { INITIALIZING, INITIALIZED, RUNNING, PAUSE, STOP };

enum SceneType { MainGame };

class PoPossibEngin
{
public:
	PoPossibEngin(const EngineConfig& engineConfig);
	~PoPossibEngin();

	// get / set

	[[nodiscard]] sf::RenderWindow& getRenderWindow() const;
	[[nodiscard]] EngineState getEngineState() const;

	[[nodiscard]] sf::Thread& getRenderThread();
	[[nodiscard]] sf::Thread& getLogicThread();

private:

	EngineState _engineState = STOP;

	sf::RenderWindow* _renderWindow = nullptr;
	sf::Thread _renderThread;
	sf::Thread _logicThread;

	sf::Clock _deltaClock;
	float _deltaTime = 0;
	void refreshDeltaTime();

	Scene* _currScene = nullptr;
public:
	void renderThreadEntry();
private:
	void logicThreadEntry();

	void loadScene(SceneType sceneType);
};

#endif //ENGINE_HPP