#include "Server.hpp"

Server::Server(const HostSettings& hostSettings, PoPossibEngin* engine) :
	_hostSettings(hostSettings),
	_serverSocket(ServerSocket(this)),
	_engine(engine),
	_serverThread(sf::Thread(&Server::threadEntry, this))
{

}

const HostSettings& Server::getHostSettings() const { return _hostSettings; }
const sf::Thread& Server::getServerThread() const { return _serverThread; }

void Server::threadEntry()
{

}
