#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

struct EngineConfig
{
	struct WindowConfig
	{
		VideoMode videoMode = VideoMode(800, 800);
		string title = "Render Window";
		unsigned int style = Style::Default;
		bool enableIMGUI = true;
		unsigned int framerateLimit = 0; // 0 mean automatic (verticalSyncEnabled = true)
	};

	WindowConfig windowConfig;
};

class Engine
{
public:
	Engine();
	Engine(const EngineConfig& engineConfig);
	~Engine();

	// get / set

	RenderWindow& getRenderWindow() const;

private:
	RenderWindow* _renderWindow = nullptr;
	Thread _renderThread;
	Thread _logicThread;

	void renderThreadEntry();
	void logicThreadEntry();
};

#endif //ENGINE_HPP