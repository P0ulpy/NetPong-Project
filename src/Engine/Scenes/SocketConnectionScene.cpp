#include "SocketConnectionScene.hpp"

#include "imgui.h"
#include "../SocketsManagement/Server/Server.hpp"

SocketConnectionScene::SocketConnectionScene(PoPossibEngin& poPossibEngin) :
	Scene(poPossibEngin, SceneConfig())
{

}

SocketConnectionScene::~SocketConnectionScene()
{

}

void SocketConnectionScene::start()
{

}

void SocketConnectionScene::update(const float& deltaTime) {}

void SocketConnectionScene::render(sf::RenderTarget* renderTarget)
{
	ImGui::Begin("Socket connection Info");

	if (ImGui::Button("Host game"))
	{
		hostWindowOpen = true;
		joinWindowOpen = false;
	}

	if (ImGui::Button("Join game"))
	{
		hostWindowOpen = false;
		joinWindowOpen = true;
	}

	if (hostWindowOpen)
		displayHostWindow();
	if (joinWindowOpen)
		displayJoinWindow();

	ImGui::End();

	if(_engine->getSocketManager().getServerInstance() != nullptr)
	{
		auto server = _engine->getSocketManager().getServerInstance();

		ImGui::Begin("Server Infos");

		ImGui::BeginChild("Clients list");

		for(auto client : server->getServerSocket().getClients())
		{
			ImGui::Text(client.first.c_str());
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

		if(ImGui::Button("Create lobby"))
		{
			_engine->getSocketManager().startServer(HostSettings(
				name,
				port
			));
		}
	}
}

void SocketConnectionScene::displayJoinWindow()
{
	if (ImGui::CollapsingHeader("Join"))
	{
		char ip[16] = "127.0.0.1";
		ImGui::InputTextWithHint("Lobby ip", "127.0.0.1", ip, IM_ARRAYSIZE(ip));

		int port = 25565;
		ImGui::InputInt("Lobby port", &port);

		if (ImGui::Button("Join lobby"))
		{
			_engine->getSocketManager().connectClient(ClientConnectionSettings(
				ip,
				port
			));
		}
	}
}
