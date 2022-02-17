#pragma once

#include "../Scene.hpp"

class LobbyScene : public Scene
{
public:
	LobbyScene(PoPossibEngin& poPossibEngin);
	~LobbyScene();

	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* renderTarget = nullptr);

private:
};