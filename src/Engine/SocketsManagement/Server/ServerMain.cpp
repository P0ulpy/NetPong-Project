#include "ServerMain.hpp"

#include <utility>
#include "../../../Logger/Logger.hpp"
#include "Objects/Character.hpp"

using namespace Server;

uint64_t ServerMain::_idIncrement = -1;

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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / SERVER_TICK_RATE));

        if(!_isGameStarted && _clients.size() > 1) _isGameStarted = true;

        sf::Packet sceneState = buildSceneState();
        _serverSocket->sendToAll(SocketEvents::SceneUpdate, sceneState);
    }
}

sf::Packet ServerMain::buildSceneState()
{
    sf::Packet sceneState;

    sceneState << _isGameStarted;

    for(auto& object : _syncableObjects)
    {
        sceneState << object->getId();
        sceneState << (int)object->getType();
        sceneState << object->getController().getId();

        sf::Packet objectStatePacket = object->getObject()->sync();
        sceneState.append(objectStatePacket.getData(), objectStatePacket.getDataSize());
    }

    return sceneState;
}

void ServerMain::onSceneUpdate(sf::Packet& packet)
{
    std::lock_guard lock(_mutex);

    while(!packet.endOfPacket())
    {
        int id = -1;
        packet >> id;

        int typeInt;
        packet >> typeInt;
        auto type = (SyncableObjectType) typeInt;

        int controlInt;
        packet >> controlInt;
        auto control = (SyncableObjectControl)controlInt;

        if (id < 0)
        {
            Logger::Err("Invalid syncable Object id : " + std::to_string(id));
            return;
        }

        if(_syncableObjects[id])
        {
            _syncableObjects[id]->getObject()->applySync(packet);
        }
        else
        {
            Logger::Err("Trying to apply synchronisation to an unknown object id :" + std::to_string(id));
        }

        //std::stringstream debugStream;
        //debugStream << "Receiving :\n{id:" << id << ", " << "type:" << type << ", control:" << control << ", data: {";
        //debugStream << "} }";
        //Logger::Log(debugStream.str());
    }
}

constexpr int pongBallPerPlayer = 10;

void ServerMain::createCharacter(Client& clientFor)
{
    std::lock_guard lock(_mutex);

    auto character = std::make_shared<Character>();
    _characters.push_back(character);

    _syncableObjects.push_back(
            std::make_shared<SyncableObject>(++_idIncrement, SyncableObjectType::CharacterType, character.get(), clientFor));

    for(int i = 0; i < pongBallPerPlayer; i++)
    {
        createPongBall(clientFor);
    }
}

void ServerMain::createPongBall(Client& clientFor) {
    auto pongBall = std::make_shared<PongBall>();
    _pongBalls.push_back(pongBall);

    _syncableObjects.push_back(
            std::make_shared<SyncableObject>(++_idIncrement, SyncableObjectType::PongBallType, pongBall.get(),clientFor));
}