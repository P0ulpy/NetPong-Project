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
    _clientsSocketSelector.clear();
	_connectionsListenThread.terminate();
	_listenEventsThread.terminate();
}

std::map<std::string, std::unique_ptr<Client>>& ServerSocket::getClients() { return _clients; }
bool ServerSocket::isReady() const { return _ready; }

void ServerSocket::registerListeners(Client& client)
{
    on(SocketEvents::PlayerSendSettings, [&client](sf::Packet packet) -> void
    {
        std::string name;
        packet >> name;
        int color = -1;
        packet >> color;

        // TODO : Set Player default position / values / color
        Logger::Log("New PlayerConnected : " + name);

        client.setSettings({ name, color });
    });

    on(SocketEvents::SceneUpdate, [this](sf::Packet packet) -> void
    {
        _server->onSceneUpdate(packet);
    });
}

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
            delete newClientSocket;
			continue;
		}

		// TODO : génération d'UUID de type non michel
        std::string UUID = "michel " + std::to_string(_clients.size());

        {
            std::lock_guard lockGuard(_mutex);

            _clients[UUID] = std::make_unique<Client>(UUID, newClientSocket);
            _clientsSocketSelector.add(*newClientSocket);
        }

        registerListeners(*_clients[UUID]);
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

                if(_break)
                    break;
			}
		}
        else
        {

        }
	}
}

void ServerSocket::send(sf::TcpSocket& socket, SocketEvents event, const sf::Packet& data)
{
    sf::Packet packet;
    packet << (int) event;
    packet.append(data.getData(), data.getDataSize());

    //Logger::Log("ServerSocket : Emitting " + std::to_string(packet.getDataSize()) + "o of data for event " + std::to_string(event));

    std::lock_guard lockGuard(_mutex);

    if (socket.send(packet/*, _clientConnectionSettings.ip, _clientConnectionSettings.port*/) !=
        sf::Socket::Done)
    {
        Logger::Err("Error while sending data | dataSize : " + std::to_string(packet.getDataSize()) + 'o');
        return;
    }
}

void ServerSocket::sendToAll(SocketEvents event, const sf::Packet& data)
{
    for(const auto& socketClientPair : _clients)
    {
        send(*socketClientPair.second->getSocket(), event, data);
    }
}

void ServerSocket::onClientConnection(Client& client)
{
    Logger::Log(
            std::string("New Client Connected | ip : ")
            + client.getSocket()->getRemoteAddress().toString()
            + " id : " + client.getId()
    );

	sf::Packet initSocketConfig;
    initSocketConfig << client.getId();

    _server->createCharacter(client);
    send(*client.getSocket(), SocketEvents::Connected, initSocketConfig);
}

void ServerSocket::onClientDisconnect(Client &client)
{
    Logger::Log("Client disconnected | id : " + client.getId());

    _clientsSocketSelector.remove(*client.getSocket());
    _clients.erase(client.getId());
}
