#pragma once

#include "Scene.hpp"

class SocketConnectionScene : public Scene
{
public:
	SocketConnectionScene(PoPossibEngin& poPossibEngin);
	~SocketConnectionScene();

	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* renderTarget = nullptr);

private:
	bool joinWindowOpen = false;
	bool hostWindowOpen = false;

	void hostWindow();
	void joinWindow();
};