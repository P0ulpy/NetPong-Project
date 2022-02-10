#include "ClientSocket.hpp"
#include "../../../Logger/Logger.hpp"

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
const EventEmitter& ClientSocket::getEventEmitter() const { return _eventEmitter; }

void ClientSocket::registerListeners()
{
    _eventEmitter.on(SocketEvents::Connected, [this](sf::Packet packet) -> void { onConnected(packet); });
    _eventEmitter.on(SocketEvents::Disconnected, [this](sf::Packet packet) -> void
    {
        Logger::Log("Disconnected");
    });
    _eventEmitter.on(SocketEvents::NewPlayerConnected, [this](sf::Packet packet) -> void
    {
        Logger::Log("New Player connected");
    });
	_listenThread.launch();
}

[[noreturn]] void ClientSocket::listenEvents()
{
    Logger::SetThreadLabel("ClientSocket-Listen");

    while(true)
	{
		sf::Packet packet;
		if(_socket.receive(packet) != sf::Socket::Done)
		{
			Logger::Err("Error during packet reception");
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

void ClientSocket::onConnected(sf::Packet packet)
{
	std::string socketID;
	if (!(packet >> socketID))
	{
		Logger::Err("Error can't decapsulate client socketID" + socketID);
		return;
	}

	_id = socketID;
}

void ClientSocket::send(SocketEvents event, const sf::Packet& data)
{
    // we asynchronously send data
    sf::Thread sendThread = sf::Thread([this, &event, &data]()
    {
        sf::Packet packet;
        packet << (int)event;
        packet << data;

        Logger::Log("ClientSocket : Sending " + std::to_string(packet.getDataSize()) + "o of data for event" + std::to_string(event));

        std::lock_guard lockGuard(_mutex);

        if (_socket.send(packet/*, _clientConnectionSettings.ip, _clientConnectionSettings.port*/) != sf::Socket::Done)
        {
            Logger::Err("Error while sending data | dataSize : " + std::to_string(packet.getDataSize()));
        }
    });

    sendThread.launch();
}
