#include "ClientSocket.hpp"
#include "../../../Logger/Logger.hpp"
#include "PlayerSettings.hpp"

#include <iostream>

ClientSocket::ClientSocket(const ClientConnectionSettings& clientConnectionSettings, PoPossibEngin* engine)
	: _clientConnectionSettings(clientConnectionSettings)
	, _engine(engine)
	, _listenThread(sf::Thread(&ClientSocket::listenEvents, this))
{
	if(_socket.connect(
		clientConnectionSettings.ip, 
		clientConnectionSettings.port,
        sf::milliseconds(clientConnectionSettings.connectionTimeout)
	) != sf::Socket::Status::Done)
	{
        Logger::Err("can't connect to remote");
        return;
    }

	Logger::Log("Connected to remote " + clientConnectionSettings.ip + ":" + std::to_string(clientConnectionSettings.port));

	registerListeners();
}

ClientSocket::~ClientSocket()
{
    _eventEmitter.emit(Disconnected);
	_listenThread.terminate();
}

const ClientConnectionSettings& ClientSocket::getClientConnectionSettings() const { return _clientConnectionSettings; }
const EventEmitter* ClientSocket::getEventEmitter() const { return &_eventEmitter; }
bool ClientSocket::isReady() const { return _ready; }

void ClientSocket::registerListeners()
{
    Logger::Log("registering listeners for client socket");

    _eventEmitter.on(SocketEvents::Connected, [this](const sf::Packet& packet) -> void { onConnected(packet); });
    _eventEmitter.on(SocketEvents::Disconnected, [this]() -> void
    {
        Logger::Log("Disconnected");
    });
    _eventEmitter.on(SocketEvents::NewPlayerConnected, [this](const sf::Packet& packet) -> void
    {
        Logger::Log("New Player connected");
    });
    _eventEmitter.on(SocketEvents::SceneUpdate, [this](const sf::Packet& packet) -> void { onSceneUpdate(packet); });

	_listenThread.launch();
}

[[noreturn]] void ClientSocket::listenEvents()
{
    Logger::SetThreadLabel("ClientSocket-Listen");
    Logger::Log("Start listening events for ClientSocket");

    while(true)
	{
		sf::Packet packet;
		if(_socket.receive(packet) != sf::Socket::Done)
		{
			Logger::Err("Error during packet reception");
            continue;
		}

		int eventID;
		if(packet >> eventID)
		{
			if (eventID >= SocketEvents::Count || eventID < 0)
			{
				Logger::Err("Can't emit event \"" + std::to_string(eventID) + "\" eventID is invalid");
			}

            std::lock_guard lockGuard(_mutex);

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
    Logger::Log(&"Firing Net event :" [(int)event]);

    // we asynchronously send data
    sf::Thread sendThread = sf::Thread([this, &event, &data]()
    {
        sf::Packet packet;
        packet << (int)event;
        packet.append(data.getData(), data.getDataSize());

        Logger::Log("ClientSocket : Sending " + std::to_string(packet.getDataSize()) + "o of data for event" + std::to_string(event));

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

    Logger::Log("Client socket connected and ready");

    _ready = true;
}

void ClientSocket::onSceneUpdate(sf::Packet packet)
{

}
