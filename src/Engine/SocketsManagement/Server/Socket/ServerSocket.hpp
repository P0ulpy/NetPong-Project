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
	[[nodiscard]] const std::map<std::string, sf::TcpSocket*>& getClients() const;
	
private:
	Server* _server = nullptr;

	sf::TcpListener _listener;

	EventEmitter _eventEmitter;
	void registerListeners();

	sf::Mutex mutex;

	sf::Thread _connectionsListenThread;
	void connectionsListenEntry();

	sf::Thread _listenEventsThread;
	void listenEvents();
	void propagateEvent(sf::TcpSocket& socket, sf::Packet& packet);

	std::map<std::string, sf::TcpSocket*> _clients;
	sf::SocketSelector _clientsSocketSelector;

	void onClientConnection(sf::TcpSocket* clientSocket);
};