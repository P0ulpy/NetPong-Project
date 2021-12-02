#include "SocketConnectionScene.hpp"

#include "imgui.h"

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
		hostWindow();
	if (joinWindowOpen)
		joinWindow();

	ImGui::End();
}

void SocketConnectionScene::hostWindow()
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

void SocketConnectionScene::joinWindow()
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
