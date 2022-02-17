#include "SocketConnectionScene.hpp"

#include "imgui.h"
#include "../../SocketsManagement/Server/ServerMain.hpp"
#include "../../SocketsManagement/Client/ClientSocket.hpp"
#include "../../../Logger/Logger.hpp"
//#include "../SocketsManagement/PacketOverload.hpp"

SocketConnectionScene::SocketConnectionScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{

}

SocketConnectionScene::~SocketConnectionScene() = default;

void SocketConnectionScene::start()
{

}

void SocketConnectionScene::update(const float& deltaTime) {}

void SocketConnectionScene::render(sf::RenderTarget* renderTarget)
{
	ImGui::Begin("Socket connection Info");

	if (ImGui::Button("Host game"))
    {
        _hostWindowOpen = true;
        _joinWindowOpen = false;
	}

	if (ImGui::Button("Join game"))
	{
		_hostWindowOpen = false;
		_joinWindowOpen = true;
	}

	if (_hostWindowOpen)
		displayHostWindow();
	if (_joinWindowOpen)
		displayJoinWindow();

	ImGui::End();

	if(_poPossibEngin->getSocketManager()->getServerInstance())
	{
		auto server = _poPossibEngin->getSocketManager()->getServerInstance();

		ImGui::Begin("ServerMain Infos");

		ImGui::BeginChild("Clients list");

		for(auto& client : server->getServerSocket()->getClients())
		{
			ImGui::Text("%s", client.first.c_str());
		}

		ImGui::EndChild();

		ImGui::End();
	}
}

void SocketConnectionScene::displayHostWindow()
{
	if (ImGui::CollapsingHeader("Host"))
	{
		char name[50] = "New Lobby";
		ImGui::InputTextWithHint("Lobby name", "Lobby name", name, IM_ARRAYSIZE(name));

		int port = 25565;
		ImGui::InputInt("Lobby port", &port);

        if(ImGui::Button("Host game") || !_oui)
		{
			_poPossibEngin->getSocketManager()->startServer(HostSettings(
				name,
				port
			));

            _oui = true;
		}
	}
}

void SocketConnectionScene::displayJoinWindow()
{
    auto socketManager = _poPossibEngin->getSocketManager();

	if (ImGui::CollapsingHeader("Join"))
	{
        auto clientSocket = socketManager->getSocketClient();

        if(!clientSocket)
        {
            char ip[16] = "127.0.0.1";
            ImGui::InputTextWithHint("Lobby ip", "127.0.0.1", ip, IM_ARRAYSIZE(ip));

            int port = 25565;
            ImGui::InputInt("Lobby port", &port);

            if (ImGui::Button("Connect") && !_joining)
            {
                _joining = true;

                Logger::Log("start connecting Client");
                socketManager->connectClient(ClientConnectionSettings(
                        ip,
                        port, std::string(), 0));
            }
        }
        else
        {
            // Connecting to socket server
            if(!clientSocket->isReady())
            {
                ImGui::Text("Connecting...");
            }
            // Socket is connected
            else
            {
                char pseudo[16] = "Xx_pseudo_xX";
                ImGui::InputTextWithHint("Pseudo", "Player...", pseudo, IM_ARRAYSIZE(pseudo));

                if (ImGui::Button("Join game") || !_oui2LeRetour)
                {
                    _oui2LeRetour = true;
                    Logger::Log("Start Joining Game");

                    clientSocket->_playerSettings.name = pseudo;

                    sf::Packet settings;
                    settings << clientSocket->_playerSettings.name;
                    clientSocket->send(SocketEvents::PlayerSendSettings, settings);
                }
            }
        }
	}
}
