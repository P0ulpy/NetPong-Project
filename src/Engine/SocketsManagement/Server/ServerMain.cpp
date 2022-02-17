#include "ServerMain.hpp"

#include <utility>
#include "../../../Logger/Logger.hpp"
#include "Objects/Character.hpp"

using namespace Server;

uint64_t ServerMain::_idIncrement = 0;

ServerMain::ServerMain(HostSettings hostSettings, PoPossibEngin* engine)
    : _hostSettings(std::move(hostSettings))
    , _engine(engine)
    , _serverSocket(new ServerSocket(this))
    , _clients(_serverSocket->getClients())
    , _serverThread(sf::Thread(&ServerMain::threadEntry, this))
{
    _serverThread.launch();
}

ServerMain::~ServerMain()
{
    _serverThread.terminate();
    delete _serverSocket;

}

const HostSettings& ServerMain::getHostSettings() const { return _hostSettings; }
ServerSocket *ServerMain::getServerSocket() const { return _serverSocket; }
const sf::Thread& ServerMain::getServerThread() const { return _serverThread; }

bool ServerMain::isReady() const
{
    if(!_serverSocket) return false;
    return _serverSocket->isReady();
}

[[noreturn]] void ServerMain::threadEntry()
{
    Logger::SetThreadLabel("ServerMainThread");

    // waiting for server socket to be ready
    while (!_serverSocket->isReady()) {}

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if(_clients.contains("michel 0"))
        {
            sf::Packet sceneState = buildSceneState();

            if(_serverSocket->getClients().contains("michel 0"))
            {
                _serverSocket->send(*_serverSocket->getClients()["michel 0"]->getSocket(), SocketEvents::SceneUpdate, sceneState);
            }
        }

        //_serverSocket->sendToAll(SocketEvents::SceneUpdate, sceneState);

        /*for(auto& clientPair : _clients)
        {
            auto id = clientPair.first;
            auto& client = *clientPair.second;
            sf::TcpSocket* socket = clientPair.second->getSocket();

        }*/
    }

}

sf::Packet ServerMain::buildSceneState()
{
    sf::Packet sceneState;

    for(auto* object : _syncableObjects)
    {
        sceneState << object->getId();
        sceneState << (int)object->getType();

        switch (object->getType())
        {
            case CharacterType:
                sceneState << 0 << 0 << 0 << 0 << 0.0f << 0;

                break;
            default:
                Logger::Warn("Unhandled SyncableObjectType :" + std::to_string((int)object->getType()));
        }
    }

    return sceneState;
}

void ServerMain::onSceneUpdate(const sf::Packet& packet)
{
    std::lock_guard lock(_mutex);

}

void ServerMain::createCharacter(Client& clientFor)
{
    std::lock_guard lock(_mutex);

    auto* character = new Character();
    _characters.push_back(character);
    _syncableObjects.push_back(new SyncableObject(++_idIncrement, SyncableObjectType::CharacterType, character, clientFor));
}

void ServerMain::deleteCharacter()
{
    std::lock_guard lock(_mutex);

}
