#include "ClientSocket.hpp"
#include "../SocketEvents.hpp"
#include "../../../Logger/Logger.hpp"

#include <iostream>

ClientSocket::ClientSocket(const ClientConnectionSettings& clientConnectionSettings, PoPossibEngin* engine)
	: _clientConnectionSettings(clientConnectionSettings)
	, _engine(engine)
	, _listenThread(sf::Thread(&ClientSocket::listenEvents, this))
{
	if(_socket.connect(
		clientConnectionSettings.ip, 
		clientConnectionSettings.port
	) != sf::Socket::Status::Done)
	{
        Logger::Err("can't connect to remote");
        return;
    }

	Logger::Log(
            "Connected to remote "
            + clientConnectionSettings.ip
            + ":"
            + std::to_string(clientConnectionSettings.port)
    );

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
	_listenThread.launch();
}

[[noreturn]] void ClientSocket::listenEvents()
{
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
			if (eventID > SocketEvents::Count || eventID < 0)
			{
				Logger::Err(
                    std::string("Can't emit event \"")
                    + std::to_string(eventID)
                    + std::string("\" eventID is invalid")
                );
			}

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
		Logger::Err("Error can't decapsulate client socketID");
		return;
	}

	_id = socketID;

	// TESTS

	std::string message;
	packet >> message;

	Logger::Log("OnConnected : " + message);

	sf::Packet sendPacket;

	if (_socket.send(packet) != sf::Socket::Done)
	{
		Logger::Err("Error while sending data : " + message);
	}

	// TESTS
}