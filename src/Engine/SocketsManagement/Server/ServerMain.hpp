#pragma once

#include <SFML/System.hpp>

#include "../SocketsManager.hpp"
#include "Socket/ServerSocket.hpp"

class PoPossibEngin;

namespace Server
{
    class ServerMain
    {
    public:
        ServerMain(HostSettings hostSettings, PoPossibEngin *engine);
        ~ServerMain();

        [[nodiscard]] const HostSettings &getHostSettings() const;
        [[nodiscard]] const sf::Thread &getServerThread() const;
        [[nodiscard]] const ServerSocket *getServerSocket() const;

        [[nodiscard]] bool isReady() const;

    private:
        HostSettings _hostSettings;

        ServerSocket* _serverSocket = nullptr;

        PoPossibEngin *_engine = nullptr;

        sf::Thread _serverThread;
        [[noreturn]] void threadEntry();
    };
}