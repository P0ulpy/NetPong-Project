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

        [[nodiscard]] const HostSettings &getHostSettings() const;
        [[nodiscard]] const sf::Thread &getServerThread() const;
        [[nodiscard]] const ServerSocket &getServerSocket() const;

    private:
        HostSettings _hostSettings;
        ServerSocket _serverSocket;

        PoPossibEngin *_engine = nullptr;

        sf::Thread _serverThread;
        [[noreturn]] void threadEntry();
    };
}