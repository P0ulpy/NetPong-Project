#include "SocketsManager.hpp"

#include "Server/ServerMain.hpp"
#include "Client/ClientSocket.hpp"
#include "../../Logger/Logger.hpp"

SocketManager::SocketManager(PoPossibEngin& engine)
    : _engine(engine)
{

}

SocketManager::~SocketManager()
{
	disconnectAll();
}

void SocketManager::disconnectAll()
{
	Logger::Log("Disconnecting all Sockets");

	delete _serverInstance;
	delete _socketClient;
}

PoPossibEngin& SocketManager::getEngine() const { return _engine; }

void SocketManager::startServer(const HostSettings& hostSettings)
{
	_serverInstance = new Server::ServerMain(hostSettings, &_engine);
}

ClientSocket* SocketManager::connectClient(const ClientConnectionSettings& clientConnectionSettings)
{
	return _socketClient = new ClientSocket(clientConnectionSettings, &_engine);
}
ClientSocket* SocketManager::getSocketClient() const
{
	return _socketClient;
}
Server::ServerMain* SocketManager::getServerInstance() const
{
	return _serverInstance;
}
bool SocketManager::isHost() const { return (_serverInstance); }

bool SocketManager::isClient() const { return (_socketClient); }
