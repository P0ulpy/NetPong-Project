#pragma once

#include "../Scene.hpp"

class SocketConnectionScene : public Scene
{
public:
	explicit SocketConnectionScene(PoPossibEngin& poPossibEngin);
	~SocketConnectionScene() override;

	void start() final;
	void update(const float& deltaTime) final;
	void render(sf::RenderTarget* renderTarget = nullptr) final;

private:
	bool _joinWindowOpen = false;
    bool _hostWindowOpen = false;
    bool _joining = false;

    bool _oui = false;
    bool _oui2LeRetour = false;

	void displayHostWindow();
	void displayJoinWindow();
};