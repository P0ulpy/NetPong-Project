#pragma once

#include <SFML/Network.hpp>

#include "../../../../Utils/EventEmitter.hpp"
#include "../../SocketEvents.hpp"
#include "Client.hpp"

namespace Server
{
    class ServerMain;

    class ServerSocket : public EventEmitter {
    public:
        explicit ServerSocket(ServerMain *server);
        ~ServerSocket();

        [[nodiscard]] std::map<std::string, std::unique_ptr<Client>>& getClients();
        [[nodiscard]] bool isReady() const;

        void send(sf::TcpSocket &socket, SocketEvents event, const sf::Packet &data);
        void sendToAll(SocketEvents event, const sf::Packet &data);

    private:
        ServerMain* _server = nullptr;
        bool _ready = false;

        sf::TcpListener _listener;
        void registerListeners(sf::TcpSocket *clientSocket);

        std::mutex _mutex;
        sf::Thread _connectionsListenThread;
        [[noreturn]] void connectionsListenEntry();

        sf::Thread _listenEventsThread;
        [[noreturn]] void listenEvents();

        std::map<std::string, std::unique_ptr<Client>> _clients;
        sf::SocketSelector _clientsSocketSelector;

        void onClientConnection(Client &client);
        void onClientDisconnect(Client &client);

        friend ServerMain;
    };
}