#pragma once
#include "iostream"
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include "../../Utils/UI.hpp"
#include "../../Logger/Logger.hpp"


class MainMenuScene : public Scene
{
public:
	MainMenuScene(PoPossibEngin& poPossibEngin);
	~MainMenuScene();

	void start() final;
	void update(const float& deltaTime) final;
	void render(sf::RenderTarget* renderTarget) final;

	void Play();
	void Quit();
	void BackToMenu();
	void HostMenu();
	void JoinMenu();
	void Host();
	void Join();
	void WaitingScreen();


	void InitializeStartMenu();
	void InitializePlayMenu();
	void InitializeHostMenu();
	void InitializeJoinMenu();
	void InitializeWaitingScreen();

	void PlayerFound();
	void CheckMouse();
	void Write(const std::string& name, int size);

private:

	float calculTime = 0;
	
	UI *ui;
	sf::Font font;
	bool _isClicked = false;
	bool _playerFound = false;
	int _menusIndex = 0;
	int _indexTextBox = 0;

    const std::string host_hostNameTextDefault = "New Lobby";
    const std::string host_hostPortTextDefault = "25565";

    const std::string join_hostIpTextDefault = "127.0.0.1";
    const std::string join_hostPortTextDefault = "25565";

	const std::string wait_hostIpTextDefault = "127.0.0.1";
	const std::string wait_hostPortTextDefault = "25565";
};