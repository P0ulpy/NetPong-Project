#include "ServerSocket.hpp"

#include <iostream>

#include "../Server.hpp"
#include "../../SocketEvents.hpp"
#include "../../../../Logger/Logger.hpp"

ServerSocket::ServerSocket(Server* server) :
	_server(server),
	_connectionsListenThread(sf::Thread(&ServerSocket::connectionsListenEntry, this)),
	_listenEventsThread(sf::Thread(&ServerSocket::listenEvents, this))
{
	const HostSettings& hostSettings = server->getHostSettings();

	if(_listener.listen(hostSettings.port) != sf::Socket::Done)
	{
		Logger::Log("Error during server listner port binding");
		return;
	}

	Logger::Log(std::string("Server listening on ") + std::to_string(hostSettings.port));

	_connectionsListenThread.launch();
	_listenEventsThread.launch();
}

ServerSocket::~ServerSocket()
{
	_connectionsListenThread.terminate();
	_listenEventsThread.terminate();

	for(auto socket : _clients)
	{
		delete socket.second;
	}
}

//const EventEmitter& ServerSocket::getEventEmitter() const { return _eventEmitter; }

const std::map<std::string, sf::TcpSocket*>& ServerSocket::getClients() const { return _clients; }

[[noreturn]] void ServerSocket::connectionsListenEntry()
{
	while(true)
	{
		auto newClient = new sf::TcpSocket();

		if (_listener.accept(*newClient) != sf::Socket::Done)
		{
			Logger::Log("Erreur lors de l'acceptation du client");
			continue;
		}

		Logger::Log(std::string("Connection d'un nouveau client\nip : ") + newClient->getRemoteAddress().toString());

		// TODO : génération d'UUID
		// temp

		std::string UUID = "michel ";
		UUID += std::to_string(_clients.size());
        Logger::Log(UUID);

		mutex.lock();
		_clients[UUID] = newClient;
		_clientsSocketSelector.add(*newClient);
		onClientConnection(newClient);
		mutex.unlock();
	}
}

[[noreturn]] void ServerSocket::listenEvents()
{
	while (true)
	{
		if (_clientsSocketSelector.wait(
				sf::milliseconds(_server->getHostSettings().socketConnectionTimeout)
			)
		)
		{
			for (const auto& socketClientPair : _clients)
			{
				std::string id = socketClientPair.first;
				sf::TcpSocket* socket = socketClientPair.second;

				if (_clientsSocketSelector.isReady(*socket))
				{
					sf::Packet packet;
					sf::Socket::Status status = socket->receive(packet);

					switch (status)
					{
					case sf::Socket::Done:
						{
							//TESTS

							SocketEvents event;
							std::string data;

							packet >> (int&)event;
							packet >> data;

							std::cout
                                    << "data recieved :"
                                    << event
							        << data
                                    << std::endl;

							//TESTS
						}
					break;

					case sf::Socket::NotReady:
						break;
					case sf::Socket::Partial:
						break;
					case sf::Socket::Disconnected:
						_clientsSocketSelector.remove(*socket);
						break;
					case sf::Socket::Error:
						_clientsSocketSelector.remove(*socket);
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
void ServerSocket::emit(SocketEvents event, sf::TcpSocket& socket, sf::Packet data)
{


	/*sf::Packet packet;
	if (socket.receive(packet) != sf::Socket::Done)
	{
		std::cout << "Error during packet reception" << std::endl;
	}

	int eventID;
	if (packet >> eventID)
	{
		_eventEmitter.emit(eventID, packet);
	}
	else
	{
		std::cout << "Error can't decapsulate eventID" << std::endl;
	}*/
}

void ServerSocket::registerListeners(sf::TcpSocket* clientSocket)
{
	
}

void ServerSocket::onClientConnection(sf::TcpSocket* clientSocket)
{
	Logger::Log("Client connecte !");

	// TESTS

	/*sf::Packet hello;
	hello << SocketEvents::Connected;
	hello << "Hello World";

	if(clientSocket->send(hello) != sf::Socket::Done)
	{
		std::cout << "Error during hello" << std::endl;
	}

	sf::Packet packet;
	if (clientSocket->receive(packet) != sf::Socket::Done)
	{
		std::cout << "Error during packet reception" << std::endl;
	}

	SocketEvents event;
	std::string data;

	packet >> (int&)event;
	packet >> data;

	std::cout << "data recieved :" << std::endl;
	std::cout << event << std::endl;
	std::cout << data << std::endl;

	*/
}
