#include "ServerMain.hpp"

#include <utility>
#include "../../../Logger/Logger.hpp"

using namespace Server;

ServerMain::ServerMain(HostSettings hostSettings, PoPossibEngin* engine)
    : _hostSettings(std::move(hostSettings))
	, _serverSocket(new ServerSocket(this))
	, _engine(engine)
	, _serverThread(sf::Thread(&ServerMain::threadEntry, this))
{

}

ServerMain::~ServerMain() {

}

const HostSettings& ServerMain::getHostSettings() const { return _hostSettings; }
const ServerSocket* ServerMain::getServerSocket() const { return _serverSocket; }
const sf::Thread& ServerMain::getServerThread() const { return _serverThread; }

bool ServerMain::isReady() const { return (_serverSocket) ? _serverSocket->isReady() : false; }

[[noreturn]] void ServerMain::threadEntry()
{
    Logger::SetThreadLabel("ServerMainThread");
}
