#pragma once

#include "Scene.hpp"

class SocketConnectionScene : public Scene
{
public:
	explicit SocketConnectionScene(PoPossibEngin& poPossibEngin);
	~SocketConnectionScene() override;

	void start() final;
	void update(const float& deltaTime) final;
	void render(sf::RenderTarget* renderTarget = nullptr) final;

private:
	bool joinWindowOpen = false;
	bool hostWindowOpen = false;

	void displayHostWindow();
	void displayJoinWindow();
};