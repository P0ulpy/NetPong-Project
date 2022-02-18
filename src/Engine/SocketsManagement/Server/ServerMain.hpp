#pragma once

#include <SFML/System.hpp>

#include "../SocketsManager.hpp"
#include "Socket/ServerSocket.hpp"
#include "SyncManagement/SyncableObject.hpp"

class PoPossibEngin;

namespace Server
{
    class Character;

    class ServerMain
    {
    public:
        ServerMain(HostSettings hostSettings, PoPossibEngin *engine);
        ~ServerMain();

        [[nodiscard]] const HostSettings &getHostSettings() const;
        [[nodiscard]] const sf::Thread &getServerThread() const;
        [[nodiscard]] ServerSocket *getServerSocket() const;

        [[nodiscard]] bool isReady() const;

    private:
        HostSettings _hostSettings;
        ServerSocket* _serverSocket = nullptr;
        PoPossibEngin *_engine = nullptr;

        std::mutex _mutex;
        sf::Thread _serverThread;
        [[noreturn]] void threadEntry();

        std::map<std::string, std::unique_ptr<Client>>& _clients;

        static uint64_t _idIncrement;
        std::vector<SyncableObject*> _syncableObjects;
        std::vector<Character*> _characters;

        void onSceneUpdate(sf::Packet& packet);
        sf::Packet buildSceneState();

        void deleteCharacter();
        void createCharacter(Client& clientFor);

        friend ServerSocket;
    };
}