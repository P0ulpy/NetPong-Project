#pragma once

#include <SFML/System.hpp>

#include "../SocketsManager.hpp"
#include "Socket/ServerSocket.hpp"

class PoPossibEngin;

class Server
{
public:
	Server(const HostSettings& hostSettings, PoPossibEngin* engine);

	[[nodiscard]] const HostSettings& getHostSettings() const;
	[[nodiscard]] const sf::Thread& getServerThread() const;

private:
	HostSettings _hostSettings;
	ServerSocket _serverSocket;

	PoPossibEngin* _engine = nullptr;

	sf::Thread _serverThread;

	void threadEntry();
};