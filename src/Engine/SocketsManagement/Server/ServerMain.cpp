#include "ServerMain.hpp"
#include "../../../Logger/Logger.hpp"

using namespace Server;

ServerMain::ServerMain(const HostSettings& hostSettings, PoPossibEngin* engine)
    : _hostSettings(hostSettings)
	, _serverSocket(ServerSocket(this))
	, _engine(engine)
	, _serverThread(sf::Thread(&ServerMain::threadEntry, this))
{
    
}

const HostSettings& ServerMain::getHostSettings() const { return _hostSettings; }
const sf::Thread& ServerMain::getServerThread() const { return _serverThread; }
const ServerSocket& ServerMain::getServerSocket() const { return _serverSocket; }

[[noreturn]] void ServerMain::threadEntry()
{
    Logger::SetThreadLabel("ServerMainThread");
}