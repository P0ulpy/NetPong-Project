#pragma once

#include "ServerSocket.hpp"

#include <iostream>

#include "../Server.hpp"

ServerSocket::ServerSocket(Server* server) :
	_server(server),
	_connectionsListenThread(sf::Thread(&ServerSocket::connectionsListenEntry, this)),
	_packetListenThread(sf::Thread(&ServerSocket::packetListenEntry, this))
{
	const HostSettings& hostSettings = server->getHostSettings();

	if(_listener.listen(hostSettings.port) != sf::Socket::Done)
	{
		throw std::exception("Error during server listner port binding");
	}

	std::cout << "Server listening on " << hostSettings.port << std::endl;

	registerListeners();
}

ServerSocket::~ServerSocket()
{
	_connectionsListenThread.terminate();
	_packetListenThread.terminate();

	for(auto socket : _clients)
	{
		delete socket.second;
	}
}

const EventEmitter& ServerSocket::getEventEmitter() const { return _eventEmitter; }

void ServerSocket::registerListeners()
{


	_connectionsListenThread.launch();
	_packetListenThread.launch();
}

void ServerSocket::connectionsListenEntry()
{
	while(true)
	{
		auto newClient = new sf::TcpSocket();

		if (_listener.accept(*newClient) != sf::Socket::Done)
		{
			std::cout << "Erreur lors de l'acceptation du client" << std::endl;
			continue;
		}

		if(newClient->getRemoteAddress() == sf::IpAddress::None)
		{
			std::cout << "Le socket est deja connecte" << std::endl;
			continue;
		}

		// TODO : génération d'UUID
		// temp

		_clients["michel"] = newClient;
		onClientConnect(newClient);
	}
}

void ServerSocket::packetListenEntry()
{

}

void ServerSocket::onClientConnect(sf::TcpSocket* clientSocket)
{
	std::cout << "Client connecte !" << std::endl;

	sf::Packet hello;
	hello << 0;
	hello << "Hello World";

	if(clientSocket->send(hello) != sf::Socket::Done)
	{
		std::cout << "Error during hello" << std::endl;
	}
}
