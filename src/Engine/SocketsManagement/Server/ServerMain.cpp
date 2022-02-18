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

constexpr unsigned int serverTick = 30;

[[noreturn]] void ServerMain::threadEntry()
{
    Logger::SetThreadLabel("ServerMainThread");

    // waiting for server socket to be ready
    while (!_serverSocket->isReady()) {}

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(serverTick));

        sf::Packet sceneState = buildSceneState();
        _serverSocket->sendToAll(SocketEvents::SceneUpdate, sceneState);
    }
}

sf::Packet ServerMain::buildSceneState()
{
    sf::Packet sceneState;

    for(auto* object : _syncableObjects)
    {
        sceneState << object->getId();
        sceneState << (int)object->getType();
        sceneState << object->getController().getId();

        switch (object->getType())
        {
            case CharacterType:
            {
                auto& state = static_cast<Character *>(object->getObject())->state;
                sceneState << state.position.x << state.position.y << state.velocity.x << state.velocity.y << state.angle << state.angularVelocity;
            }
                break;
            default:
                Logger::Warn("Unhandled SyncableObjectType :" + std::to_string((int)object->getType()));
        }
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

        std::stringstream debugStream;
        debugStream << "Receiving :\n{id:" << id << ", " << "type:" << type << ", control:" << control << ", data: {";

        {
            int px, py;
            float vx, vy;
            float angle, angleVel;

            packet >> px;
            packet >> py;
            packet >> vx;
            packet >> vy;
            packet >> angle;
            packet >> angleVel;

            auto* character = static_cast<Character *>(_syncableObjects[id]->getObject());

            character->state = {
                    {px, py},
                    {vx, vy},
                    angle, angleVel
            };

            debugStream << "pos: {x:" << px << ", y:" << py << "}, vel: {x:" << vx << ", y:" << vy << "}, angle: " << angle << ", angleVel: " << angleVel;
        }

        debugStream << "} }";
        //Logger::Log(debugStream.str());
    }
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
