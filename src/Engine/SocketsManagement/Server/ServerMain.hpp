#pragma once

#include <SFML/System.hpp>

#include "../SocketsManager.hpp"
#include "Socket/ServerSocket.hpp"
#include "SyncManagement/SyncableObject.hpp"
#include "Objects/PongBall.hpp"

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

        bool _isGameStarted = false;

        HostSettings _hostSettings;
        ServerSocket* _serverSocket = nullptr;
        PoPossibEngin *_engine = nullptr;

        std::mutex _mutex;
        sf::Thread _serverThread;
        [[noreturn]] void threadEntry();

        std::map<std::string, std::unique_ptr<Client>>& _clients;

        static uint64_t _idIncrement;
        std::vector<std::shared_ptr<SyncableObject>> _syncableObjects;
        std::vector<std::shared_ptr<Character>> _characters;
        std::vector<std::shared_ptr<PongBall>> _pongBalls;

        void onSceneUpdate(sf::Packet& packet);
        sf::Packet buildSceneState();

        void createCharacter(Client& clientFor);
        void createPongBall(Client& clientFor);

        void checkPingBallCollisions();
        Character* checkPlayerCollision(PongBall *pongBall);

        friend ServerSocket;

    };
}