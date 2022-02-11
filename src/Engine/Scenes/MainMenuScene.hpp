#pragma once

#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
	MainMenuScene(PoPossibEngin& poPossibEngin);
	~MainMenuScene();

	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* renderTarget = nullptr);

private:
};