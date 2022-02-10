#pragma once

#include <SFML/Network.hpp>

#include "../../../../Utils/EventEmitter.hpp"
#include "../../SocketEvents.hpp"
#include "Client.hpp"

class Server;

class ServerSocket : public EventEmitter
{
public:
	explicit ServerSocket(Server* server);
	~ServerSocket();

	[[nodiscard]] const std::map<std::string, std::unique_ptr<Client>>& getClients() const;
	
private:
	Server* _server = nullptr;

	sf::TcpListener _listener;
	void registerListeners(sf::TcpSocket* clientSocket);

	std::mutex _mutex;

    sf::Thread _connectionsListenThread;
    [[noreturn]] void connectionsListenEntry();

    sf::Thread _listenEventsThread;
    [[noreturn]] void listenEvents();

	void emit(sf::TcpSocket& socket, SocketEvents event, const sf::Packet& data);
	void emitToAll(SocketEvents event, const sf::Packet& data);

	std::map<std::string, std::unique_ptr<Client>> _clients;
	sf::SocketSelector _clientsSocketSelector;

    void onClientConnection(const Client& client);
    void onClientDisconnect(const Client& client);
};