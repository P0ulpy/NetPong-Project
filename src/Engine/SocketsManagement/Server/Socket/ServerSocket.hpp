#pragma once

#include <SFML/Network.hpp>

#include "../../../../Utils/EventEmitter.hpp"

class Server;

class ServerSocket
{
public:
	ServerSocket(Server* server);
	~ServerSocket();

	[[nodiscard]] const EventEmitter& getEventEmitter() const;

private:
	Server* _server;

	sf::TcpListener _listener;

	EventEmitter _eventEmitter;
	void registerListeners();

	sf::Thread _connectionsListenThread;
	void connectionsListenEntry();

	sf::Thread _packetListenThread;
	void packetListenEntry();

	std::map<std::string, sf::TcpSocket*> _clients;

	void onClientConnect(sf::TcpSocket* clientSocket);
};