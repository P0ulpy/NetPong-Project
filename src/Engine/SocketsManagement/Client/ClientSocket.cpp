#include "ClientSocket.hpp"
#include "../../../Logger/Logger.hpp"
#include "PlayerSettings.hpp"
#include "../SyncableObjectType.hpp"
#include "../../Scenes/MainGameScene.hpp"

ClientSocket::ClientSocket(ClientConnectionSettings clientConnectionSettings, PoPossibEngin* engine)
	: _clientConnectionSettings(std::move(clientConnectionSettings))
	, _engine(engine)
	, _listenThread(sf::Thread(&ClientSocket::listenEvents, this))
    , _syncableObjectManager(this)
{
    sf::Thread connectThread([this]() -> void
    {
        if(_socket.connect(
                _clientConnectionSettings.ip,
                _clientConnectionSettings.port,
                sf::milliseconds(_clientConnectionSettings.connectionTimeout)
        ) != sf::Socket::Status::Done)
        {
            Logger::Err("can't connect to remote");
            return;
        }

        Logger::Log("Connected to remote " + _clientConnectionSettings.ip + ":" + std::to_string(_clientConnectionSettings.port));

        registerListeners();
    });

    connectThread.launch();
}

ClientSocket::~ClientSocket()
{
    // FIXME : BIG LEAK terminate crash app because mutex is loacked make some test anc cnage lock scope to fix this
    //_listenThread.terminate();
}

const ClientConnectionSettings& ClientSocket::getClientConnectionSettings() const { return _clientConnectionSettings; }
Client::SyncableObjectManager &ClientSocket::getSyncableObjectManager() { return _syncableObjectManager; }
const EventEmitter* ClientSocket::getEventEmitter() const { return &_eventEmitter; }
std::string &ClientSocket::getId() { return _id; }

bool ClientSocket::isReady() const { return _ready; }

void ClientSocket::registerListeners()
{
    Logger::Log("registering listeners for client socket");

    _eventEmitter.on(SocketEvents::Connected, [this](sf::Packet packet) -> void
    {
        onConnected(packet);
    });

    _eventEmitter.on(SocketEvents::Disconnected, [this]() -> void
    {
        Logger::Log("Disconnected");
    });

    _eventEmitter.on(SocketEvents::NewPlayerConnected, [this](sf::Packet packet) -> void
    {
        Logger::Log("New Player connected");
    });

    _eventEmitter.on(SocketEvents::SceneUpdate, [this](sf::Packet packet) -> void
    {
        if(!_ready) return;
        _syncableObjectManager.onSceneUpdate(packet);
    });

    _eventEmitter.on(SocketEvents::PongBallCollision, [this](sf::Packet packet) -> void
    {
        if(!_ready) return;

        int id = -1;
        packet >> id;

        Logger::Log("PongBallcollision id:" + std::to_string(id));
    });

    _listenThread.launch();
}

[[noreturn]] void ClientSocket::listenEvents()
{
    Logger::SetThreadLabel("ClientSocket-Listen");
    Logger::Log("Start listening events for ClientSocket");

    while(true)
	{
        //Logger::Log("Waiting for reception");

        sf::Packet packet;
		if(_socket.receive(packet) != sf::Socket::Done)
		{
			Logger::Err("Error during packet reception");
            continue;
		}

        //Logger::Log("Receiving");

        int eventIDInt;
        SocketEvents eventID;
		if(packet >> eventIDInt)
		{
            eventID = (SocketEvents)eventIDInt;

            //Logger::Log("Firring Event : " + std::to_string(eventID));

			if (eventID >= SocketEvents::Count || eventID < 0)
			{
				Logger::Err("Can't emit event \"" + std::to_string(eventID) + "\" eventID is invalid");
			}

			_eventEmitter.emit(eventID, packet);

		}
		else
		{
			Logger::Err("Error can't decapsulate eventID");
		}
	}
}

void ClientSocket::send(SocketEvents event, const sf::Packet& data)
{
    // asynchronously send data
    sf::Thread sendThread = sf::Thread([this, &event, &data]()
    {
        sf::Packet packet;
        packet << (int)event;
        packet.append(data.getData(), data.getDataSize());

        //Logger::Log("ClientSocket : Sending " + std::to_string(packet.getDataSize()) + "o of data for event : " + std::to_string(event));

        std::lock_guard lockGuard(_mutex);

        if (_socket.send(packet/*, _clientConnectionSettings.ip, _clientConnectionSettings.port*/) != sf::Socket::Done)
        {
           Logger::Err("Error while sending data | dataSize : " + std::to_string(packet.getDataSize()));
        }
    });

    sendThread.launch();
}

void ClientSocket::onConnected(sf::Packet packet)
{
	if (!(packet >> _id))
	{
		Logger::Err("Error can't decapsulate client socketID: " + _id);
		return;
	}

    Logger::Log("Client socket connected and ready : " + _id);
    _ready = true;

    sendPlayerSettings();
}

void ClientSocket::sendPlayerSettings()
{
    _playerSettings.name = _clientConnectionSettings.pseudo;

    sf::Packet settings;
    settings << _playerSettings.name;
    send(SocketEvents::PlayerSendSettings, settings);

    Logger::Log("PlayerSettings sent");
}

void ClientSocket::waitReady() const
{
    while (true) if(_ready) return;
}
