#include "ServerSocket.hpp"

#include <iostream>

#include "../ServerMain.hpp"
#include "../../../../Logger/Logger.hpp"
//#include "./../../../SocketsManagement/PacketOverload.hpp"

using namespace Server;

ServerSocket::ServerSocket(ServerMain* server)
    : _server(server)
	, _connectionsListenThread(sf::Thread(&ServerSocket::connectionsListenEntry, this))
	, _listenEventsThread(sf::Thread(&ServerSocket::listenEvents, this))
{
    const HostSettings& hostSettings = server->getHostSettings();

    if(_listener.listen(hostSettings.port) != sf::Socket::Done)
	{
        Logger::Err("Error during server listener port binding");
        return;
    }

	Logger::Log("ServerMain listening on " + std::to_string(hostSettings.port));

	_connectionsListenThread.launch();
	_listenEventsThread.launch();
}

ServerSocket::~ServerSocket()
{
	_connectionsListenThread.terminate();
	_listenEventsThread.terminate();
}

const std::map<std::string, std::unique_ptr<Client>>& ServerSocket::getClients() const { return _clients; }

bool ServerSocket::isReady() const { return false; }

[[noreturn]] void ServerSocket::connectionsListenEntry()
{
    Logger::SetThreadLabel("ServerSocket-ConnectionListen");

    _ready = true;

    while(true)
	{
		auto newClientSocket = new sf::TcpSocket();

		if (_listener.accept(*newClientSocket) != sf::Socket::Done)
		{
			Logger::Log("Erreur lors de l'acceptation du client");
			continue;
		}

		// TODO : génération d'UUID

		std::string UUID = "michel ";
		UUID += std::to_string(_clients.size());
        Logger::Log(UUID);

        {
            std::lock_guard lockGuard(_mutex);

            _clients[UUID] = std::make_unique<Client>(UUID, newClientSocket);
            _clientsSocketSelector.add(*newClientSocket);
        }

        registerListeners(_clients[UUID]->getSocket());
        onClientConnection(*_clients[UUID].get());
	}
}

[[noreturn]] void ServerSocket::listenEvents()
{
    Logger::SetThreadLabel("ServerSocket-ListenEvents");

    while (true)
	{
        if (_clientsSocketSelector.wait())
		{
            for (const auto& socketClientPair : _clients)
			{
                bool _break = false;

				std::string id = socketClientPair.first;
				Client& client = *socketClientPair.second;

				//if (_clientsSocketSelector.isReady(*client.getSocket()))
				//{
					sf::Packet packet;
					sf::Socket::Status status = client.getSocket()->receive(packet);

					switch (status)
					{
					case sf::Socket::Done:
                    // TODO : Refacto faire des fonctions
                        {
                            int eventID;
                            if(packet >> eventID)
                            {
                                if (eventID >= SocketEvents::Count || eventID < 0)
                                {
                                    Logger::Err("Can't emit event \"" + std::to_string(eventID) + "\" eventID is invalid");
                                }

                                std::lock_guard lockGuard(_mutex);
                                emit(eventID, packet);
                            }
                            else
                            {
                                Logger::Err("Error can't decapsulate eventID");
                            }
                        }
                        break;
                    case sf::Socket::Disconnected: onClientDisconnect(client); _break = true; break;
					case sf::Socket::Error:
                            Logger::Err("Error during packet reception");
                            onClientDisconnect(client);
                            _break = true;
						break;
                    }
				//}

                if(_break)
                    break;
			}
		}
        else
        {
            //Logger::Err("Wait time elapsed ?");
        }
	}
}

void ServerSocket::send(sf::TcpSocket& socket, SocketEvents event, const sf::Packet& data)
{
    sf::Thread sendThread = sf::Thread([this, &socket, &event, &data]()
    {
        sf::Packet packet;
        packet << (int) event;
        packet.append(data.getData(), data.getDataSize());

        Logger::Log("ClientSocket : Emitting " + std::to_string(packet.getDataSize()) + "o of data for event " +
                    std::to_string(event));

        std::lock_guard lockGuard(_mutex);

        if (socket.send(packet/*, _clientConnectionSettings.ip, _clientConnectionSettings.port*/) !=
            sf::Socket::Done)
        {
            Logger::Err("Error while sending data | dataSize : " + std::to_string(packet.getDataSize()) + 'o');
            return;
        }
    });

    sendThread.launch();
}

void ServerSocket::sendToAll(SocketEvents event, const sf::Packet& data)
{
    // we asynchronously send data
    sf::Thread sendThread = sf::Thread([this, &event, &data]()
    {
        for(const auto& socketClientPair : _clients)
        {
            send(*socketClientPair.second->getSocket(), event, data);
        }
    });

    sendThread.launch();
}

void ServerSocket::registerListeners(sf::TcpSocket* clientSocket)
{
    on(SocketEvents::PlayerSendSettings, [](sf::Packet packet) -> void
    {
        std::string playerSettings;
        packet >> playerSettings;
        // TODO : Set Player default position / values / color

        Logger::Log("New PlayerConnected : " + playerSettings);

        // surely unused in future
        // emitToAll(SocketEvents::NewPlayerConnected, initSocketConfig);
    });
}

void ServerSocket::onClientConnection(const Client& client)
{
    Logger::Log(
            std::string("New Client Connected | ip : ")
            + client.getSocket()->getRemoteAddress().toString()
            + " id : " + client.getId()
    );

	sf::Packet initSocketConfig;
    initSocketConfig << client.getId();

    send(*client.getSocket(), SocketEvents::Connected, initSocketConfig);
}

void ServerSocket::onClientDisconnect(const Client &client)
{
    Logger::Log("Client disconnected | id : " + client.getId());

    _clientsSocketSelector.remove(*client.getSocket());
    _clients.erase(client.getId());
}
