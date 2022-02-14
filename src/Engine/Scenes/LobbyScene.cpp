#include "LobbyScene.hpp"

#include "SocketConnectionScene.hpp"

#include "imgui.h"
#include "../SocketsManagement/Server/ServerMain.hpp"

LobbyScene::LobbyScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{

}

LobbyScene::~LobbyScene()
{

}

void LobbyScene::start()
{

}

void LobbyScene::update(const float& deltaTime) {}

void LobbyScene::render(sf::RenderTarget* renderTarget)
{
	if (_poPossibEngin->getSocketManager()->isHost())
	{
		auto server = _poPossibEngin->getSocketManager()->getServerInstance();

		ImGui::Begin("ServerMaina Infos");

		ImGui::BeginChild("Clients list");

		for (auto& client : server->getServerSocket()->getClients())
		{
			ImGui::Text("%s", client.first.c_str());
		}

		ImGui::EndChild();

		ImGui::End();
	}
}