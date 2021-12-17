#include "SocketsManager.hpp"

#include "Server/Server.hpp"
#include "Client/ClientSocket.hpp"

SocketManager::SocketManager(PoPossibEngin& engine) :
	_engine(engine)
{
	
}

SocketManager::~SocketManager()
{
	delete _serverInstance;
	delete _socketClient;
}

PoPossibEngin& SocketManager::getEngine() const { return _engine; }

void SocketManager::startServer(const HostSettings& hostSettings)
{
	_serverInstance = new Server(hostSettings, &_engine);
}

void SocketManager::connectClient(const ClientConnectionSettings& clientConnectionSettings)
{
	_socketClient = new ClientSocket(clientConnectionSettings, &_engine);
}

ClientSocket* SocketManager::getSocketClient() const
{
	return _socketClient;
}
Server* SocketManager::getServerInstance() const
{
	return _serverInstance;
}
const bool SocketManager::isHost() const { return (_serverInstance); }
const bool SocketManager::isClient() const { return (_socketClient); }
