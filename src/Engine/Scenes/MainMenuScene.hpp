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

	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* renderTarget = nullptr);

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
	void Write(std::string name, int size);

private:

	float calculTime = 0;
	
	UI *ui;
	sf::Font font;
	bool _isClicked = false;
	bool _playerFound = false;
	int _menusIndex = 0;
	int _indexTextBox = 0;

	std::string hostip = "255.255.255.255";
	int hostport = 25565;
	int joinPort = 25565;
};