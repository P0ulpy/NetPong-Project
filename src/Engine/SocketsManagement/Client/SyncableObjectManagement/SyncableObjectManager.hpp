//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP

#include <SFML/System/Thread.hpp>
#include "SFML/Network/Packet.hpp"
#include "../../SyncableObjectType.hpp"
#include <mutex>
#include <map>

class ClientSocket;

namespace Client
{
    class SyncableObject;
    enum SyncableObjectControl;

    class SyncableObjectManager
    {
    public:
        explicit SyncableObjectManager(ClientSocket *clientSocket);
        ~SyncableObjectManager();

    private:
        ClientSocket* _clientSocket = nullptr;

        std::mutex _mutex;
        sf::Thread _syncThread = sf::Thread(&SyncableObjectManager::syncThreadEntry, this);

        [[noreturn]] void syncThreadEntry();

        std::map<unsigned int, SyncableObject*> _syncableObjects;

        void onSceneUpdate(sf::Packet& packet);
        SyncableObject* createEntity(unsigned int id, SyncableObjectControl control, SyncableObjectType entityType);
        void deleteEntity(unsigned int id);

        friend ClientSocket;
    };
}


#endif //NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
