#include "MainMenuScene.hpp"

#include "../SocketsManagement/Server/ServerMain.hpp"
#include "../SocketsManagement/Client/ClientSocket.hpp"

MainMenuScene::MainMenuScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
	{
	if (!font.loadFromFile("./Assets/arial.TTF")) {
		std::cout << "no font here" << std::endl;
	}
	ui = new UI();
	InitializeStartMenu();
	InitializePlayMenu();
	InitializeHostMenu();
	InitializeJoinMenu();
	InitializeWaitingScreen();
}

void MainMenuScene::InitializeStartMenu()
{
	ui->CreateText("PlayButton", sf::Color::White, "PLAY", 300, sf::Vector2f(960, 400),font);
	ui->CreateText("QuitButton", sf::Color::White, "QUIT", 200, sf::Vector2f(960, 740), font);
}

void MainMenuScene::InitializePlayMenu()
{
	ui->CreateText("JoinButton", sf::Color::White, "JOINGAME", 100, sf::Vector2f(960, 200), font);
	ui->CreateText("HostButton", sf::Color::White, "HOSTGAME", 100, sf::Vector2f(960, 700), font);
	ui->CreateText("BackMenuButton", sf::Color::White, "MENU", 100, sf::Vector2f(200, 900), font);
}

void MainMenuScene::InitializeHostMenu()
{
	ui->CreateText("Host", sf::Color::White, "HOST", 100, sf::Vector2f(1720, 900), font);
	ui->CreateTextBox("LobbyName", font, 15, sf::Vector2f(960, 400), host_hostNameTextDefault);
	ui->CreateText("LobbyTitle", sf::Color::White, "Lobby Name", 100, sf::Vector2f(ui->Zone("LobbyName").getPosition().x, ui->Zone("LobbyName").getPosition().y - 150), font);
	ui->CreateTextBox("HostTextPort", font, 5, sf::Vector2f(960, 700), host_hostPortTextDefault);
	ui->CreateText("HostTitlePort", sf::Color::White, "Port", 100, sf::Vector2f(ui->Zone("HostTextPort").getPosition().x, ui->Zone("HostTextPort").getPosition().y - 150), font);
	
}

void MainMenuScene::InitializeJoinMenu()
{
	ui->CreateText("Join", sf::Color::White, "JOIN", 100, sf::Vector2f(1720, 900), font);
	ui->CreateTextBox("IP", font, 15, sf::Vector2f(960, 400), join_hostIpTextDefault);
	ui->CreateText("TitleIP", sf::Color::White, "IP", 100, sf::Vector2f(ui->Zone("IP").getPosition().x, ui->Zone("IP").getPosition().y - 150), font);
	ui->CreateTextBox("TextPort", font, 5, sf::Vector2f(960, 700), join_hostPortTextDefault);
	ui->CreateText("TitlePort", sf::Color::White, "Port", 100, sf::Vector2f(ui->Zone("TextPort").getPosition().x, ui->Zone("TextPort").getPosition().y - 150), font);
	
}

void MainMenuScene::InitializeWaitingScreen()
{
	ui->CreateText("Waiting", sf::Color::White, "Waiting for player to join !", 150, sf::Vector2f(960, 400), font);
	ui->CreateText("IPAddress", sf::Color::White, wait_hostIpTextDefault, 100, sf::Vector2f(960, 100), font);
	ui->CreateText("Port", sf::Color::White, wait_hostPortTextDefault, 100, sf::Vector2f(960, 250), font);
	ui->CreateText("WaitingAnimation", sf::Color::White, "...", 100, sf::Vector2f(960, 500), font);


}

MainMenuScene::~MainMenuScene()
{
	
}

void MainMenuScene::start()
{

}

void MainMenuScene::update(const float& deltaTime)
{
	calculTime += deltaTime;
	sf::Event event{};
	CheckMouse();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))_isClicked = true;
	else _isClicked = false;

	if (_menusIndex == 2)
	{
		if (getParentEngine().getInputsManager().getEvent(sf::Event::TextEntered, event))
		{
			if (_indexTextBox == 1)Write("IP", 15);
			if (_indexTextBox == 2)Write("TextPort", 5);
		}
	}
	if (_menusIndex == 3)
	{
		if (getParentEngine().getInputsManager().getEvent(sf::Event::TextEntered, event))
		{
			if (_indexTextBox == 1)Write("LobbyName", 15);
			if (_indexTextBox == 2)Write("HostTextPort", 5);
		}
	}
}

void MainMenuScene::render(sf::RenderTarget* renderTarget)
{
	//Render main menu
	if (_menusIndex == 0)
	{
		renderTarget->draw(ui->Text("PlayButton"));
		renderTarget->draw(ui->Text("QuitButton"));
	}
	if(_menusIndex == 1)
	{
		renderTarget->draw(ui->Text("JoinButton"));
		renderTarget->draw(ui->Text("HostButton"));
		renderTarget->draw(ui->Text("BackMenuButton"));
	}
	if (_menusIndex == 2)
	{
		renderTarget->draw(ui->Text("BackMenuButton"));
		renderTarget->draw(ui->Text("TitleIP"));
		renderTarget->draw(ui->Text("Join"));
		renderTarget->draw(ui->Text("TitlePort"));
		ui->RenderTextBox("TextPort", renderTarget);
		ui->RenderTextBox("IP", renderTarget);
	}
	if (_menusIndex == 3)
	{
		renderTarget->draw(ui->Text("BackMenuButton"));
		renderTarget->draw(ui->Text("Host"));
		renderTarget->draw(ui->Text("LobbyTitle"));
		renderTarget->draw(ui->Text("HostTitlePort"));
		ui->RenderTextBox("LobbyName", renderTarget);
		ui->RenderTextBox("HostTextPort", renderTarget);
	}
	if (_menusIndex == 4)
	{
		renderTarget->draw(ui->Text("Waiting"));
		renderTarget->draw(ui->Text("BackMenuButton"));
		renderTarget->draw(ui->Text("IPAddress"));
		renderTarget->draw(ui->Text("Port"));
		renderTarget->draw(ui->Text("WaitingAnimation"));

		if (!_playerFound)
		{
			WaitingScreen();
		}
	}
}


void MainMenuScene::CheckMouse() {

    auto mousePos = _poPossibEngin->getInputsManager().getMousePosition();

	if (_menusIndex == 0)
	{
		if (ui->InteractionButton("PlayButton", mousePos, _isClicked)) Play();
		if (ui->InteractionButton("QuitButton", mousePos, _isClicked)) Quit();
	}
	if (_menusIndex == 1)
	{ 
		if (ui->InteractionButton("JoinButton", mousePos, _isClicked)) JoinMenu();
		if (ui->InteractionButton("HostButton", mousePos, _isClicked)) HostMenu();
		if (ui->InteractionButton("BackMenuButton", mousePos, _isClicked)) BackToMenu();
	}
	if (_menusIndex == 2)
	{
		if (ui->InteractionButton("Join", mousePos, _isClicked)) Join();
		if (ui->InteractionButton("BackMenuButton", mousePos, _isClicked)) BackToMenu();
		if (ui->InteractionTextBox("IP", mousePos, _isClicked))_indexTextBox = 1;
		if (ui->InteractionTextBox("TextPort", mousePos, _isClicked))_indexTextBox = 2;
	}
	if (_menusIndex == 3)
	{
		if (ui->InteractionButton("Host", mousePos, _isClicked)) Host();
		if (ui->InteractionButton("BackMenuButton", mousePos, _isClicked)) BackToMenu();
		if (ui->InteractionTextBox("LobbyName", mousePos, _isClicked))_indexTextBox = 1;
		if (ui->InteractionTextBox("HostTextPort", mousePos, _isClicked))_indexTextBox = 2;
	}
	if (_menusIndex == 4)
	{
		if (ui->InteractionButton("BackMenuButton", mousePos, _isClicked)) BackToMenu();
	}
}


void MainMenuScene::Write(const std::string& name, int size)
{
	sf::Event event {};
	if (getParentEngine().getInputsManager().getEvent(sf::Event::TextEntered, event))
	{
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
        {
            if (ui->TextBox(name).getString().getSize() > 0)
            {
                std::string stringDelete = ui->TextBox(name).getString();
                stringDelete.pop_back();
                ui->TextBox(name).setString(stringDelete);
            }
        }
        else
        {
            if (ui->TextBox(name).getString().getSize() < size)
            {
                std::string stringAdd = ui->TextBox(name).getString();
                stringAdd += std::to_string(event.text.unicode);
                ui->TextBox(name).setString(stringAdd);
            }
        }
	}
	
}

void MainMenuScene::Quit(){ getParentEngine().stop(); }
void MainMenuScene::BackToMenu()
{
    _poPossibEngin->getSocketManager()->disconnectAll();
    _menusIndex = 0;
}

void MainMenuScene::Play() { _menusIndex = 1; }
void MainMenuScene::HostMenu() { _menusIndex = 3; }
void MainMenuScene::JoinMenu() { _menusIndex = 2; }

void MainMenuScene::WaitingScreen()
{
	if (calculTime > 0) {
		ui->Text("WaitingAnimation").setString(".");
	}
	if(calculTime > 0.5) {
		ui->Text("WaitingAnimation").setString("..");
	}
	if (calculTime > 1) {
		ui->Text("WaitingAnimation").setString("...");
	}
	if (calculTime > 1.5)
	{
		calculTime = 0;
	}
}
void MainMenuScene::Host()
{
	std::string portStr = ui->TextBox("HostTextPort").getString();
    std::string lobbyName = ui->TextBox("LobbyName").getString();
    std::string pseudo = ui->TextBox("LobbyName").getString();

	ui->Text("Port").setString(portStr);
	auto hostport = (uint16_t)std::stoi(portStr);
	
	getParentEngine().getSocketManager()->startServer({
		ui->TextBox("LobbyName").getString(), hostport
    });
	
	while (!getParentEngine().getSocketManager()->getServerInstance()->isReady())
    {
        Logger::Log("creating internal server...");
    }

    ui->Text("IPAddress").setString("127.0.0.1");
    ui->Text("Port").setString(portStr);

	auto clientSocket = getParentEngine().getSocketManager()->connectClient({
		"127.0.0.1",
        (uint16_t)hostport
    });

    while(!clientSocket->isReady())
    {
        Logger::Log("connecting to internal server...");
    }

    // FIXME : ne pas charger directement la scene
    _poPossibEngin->loadScene(SceneType::MainGame);
	//_menusIndex = 4;
}

void MainMenuScene::Join()
{
	//JoinGame
	std::string tostringIP = ui->TextBox("IP").getString();
	std::string tostringPORT = ui->TextBox("TextPort").getString();

	getParentEngine().getSocketManager()->connectClient({
		tostringIP,
        (uint16_t)std::stoi(tostringPORT)
    });
}

void MainMenuScene::PlayerFound()
{
	_playerFound = true;
}









