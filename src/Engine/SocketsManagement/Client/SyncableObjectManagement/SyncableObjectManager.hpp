//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP


#include <map>
#include <string>
#include <mutex>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/Packet.hpp>
#include "../../SyncableObjectOptions.hpp"

class ClientSocket;
class Character;

namespace Engine {
    class ControllerBase;
}

namespace Client
{
    class SyncableObject;

    class SyncableObjectManager
    {
    public:
        explicit SyncableObjectManager(ClientSocket *clientSocket);
        ~SyncableObjectManager();

        static std::map<SyncableObjectType, unsigned int> objectTypesInSceneUpdate;
        static bool isGameStarted;

        std::map<int, SyncableObject*>& getSyncableObjects();
        Character *getCharacter(const std::string &controllerID);
        Engine::ControllerBase *getCharacterController(const std::string &controllerID);

    private:

        ClientSocket* _clientSocket = nullptr;

        std::mutex _mutex;
        sf::Thread _syncThread = sf::Thread(&SyncableObjectManager::syncThreadEntry, this);

        [[noreturn]] void syncThreadEntry();

        std::map<int, SyncableObject*> _syncableObjects;

        void onSceneUpdate(sf::Packet& packet);
        SyncableObject* createEntity(const SyncableObjectOptions& options, sf::Packet& packet);
        void deleteEntity(unsigned int id);

        friend ClientSocket;
    };
}


#endif //NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
