#include "ClientSocket.hpp"

#include <iostream>

ClientSocket::ClientSocket(const ClientConnectionSettings& clientConnectionSettings, PoPossibEngin* engine) :
	_clientConnectionSettings(clientConnectionSettings),
	_engine(engine),
	_listenThread(sf::Thread(&ClientSocket::listenEntry, this))
{
	if(_socket.connect(clientConnectionSettings.ip, clientConnectionSettings.port) != sf::Socket::Status::Done)
	{
		throw std::exception("can't connect to remote");
	}

	std::cout << "Connected to remote " << clientConnectionSettings.ip << ":" << clientConnectionSettings.port << std::endl;

	registerListeners();
}

ClientSocket::~ClientSocket()
{
	_listenThread.terminate();
}

const ClientConnectionSettings& ClientSocket::getClientConnectionSettings() const { return _clientConnectionSettings; }
const EventEmitter& ClientSocket::getEventEmitter() const { return _eventEmitter; }

void ClientSocket::registerListeners()
{
	_eventEmitter.on(0, [this](sf::Packet packet)
	{
		std::string hello;
		packet >> hello;

		onHelloWorld(hello);
	});

	_listenThread.launch();
}

void ClientSocket::listenEntry()
{
	while(true)
	{
		sf::Packet packet;
		if(_socket.receive(packet) != sf::Socket::Done)
		{
			std::cout << "Error during packet reception" << std::endl;
		}

		int eventID;
		if(packet >> eventID)
		{
			_eventEmitter.emit(eventID, packet);
		}
		else
		{
			std::cout << "Error can't decapsulate eventID" << std::endl;
		}
	}
}

void ClientSocket::onHelloWorld(std::string hello)
{
	std::cout << "YEY : " << hello << std::endl;
}