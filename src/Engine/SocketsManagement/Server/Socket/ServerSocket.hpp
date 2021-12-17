#pragma once

#include <SFML/Network.hpp>

#include "../../../../Utils/EventEmitter.hpp"

#include "../../SocketEvents.hpp"

class Server;

class ServerSocket
{
public:
	ServerSocket(Server* server);
	~ServerSocket();

	//[[nodiscard]] const EventEmitter& getEventEmitter() const;
	[[nodiscard]] const std::map<std::string, sf::TcpSocket*>& getClients() const;
	
private:
	Server* _server = nullptr;

	sf::TcpListener _listener;

	void registerListeners(sf::TcpSocket* clientSocket);

	sf::Mutex mutex;

	sf::Thread _connectionsListenThread;
	void connectionsListenEntry();

	sf::Thread _listenEventsThread;
	void listenEvents();

	void emit(SocketEvents event, sf::TcpSocket& socket, sf::Packet data);

	void emitToAll(SocketEvents event, sf::Packet& packet, sf::Packet data);

	std::map<std::string, sf::TcpSocket*> _clients;
	sf::SocketSelector _clientsSocketSelector;

	void onClientConnection(sf::TcpSocket* clientSocket);
};